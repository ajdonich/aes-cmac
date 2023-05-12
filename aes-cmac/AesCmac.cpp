#include "AesCmac.h"
#include <cmath>

// Constants
const unsigned char const_Zero[16] = {0};
const unsigned char const_Rb[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0x87};
const unsigned int const_Bsize = 16;

// Implementation
AesCmac::AesCmac(unsigned char key[16]) : bufferlen(128), msglen(0) {
    memcpy(K, key, 16);
    aes128Cipher.setKey(K, 16);
    msg = new unsigned char[bufferlen](); // Gets zeroed-out on ctor
}

AesCmac::~AesCmac() {
    delete [] msg;
}

void AesCmac::append(const unsigned char *data, size_t datalen) {
    if (datalen > 0) {
        if (msglen + datalen > bufferlen) {            
            size_t nxtblen = ceil((msglen + datalen) / 128.0) * 128;
            unsigned char *buffer = new unsigned char[nxtblen]();
            memcpy(buffer, msg, msglen);

            delete [] msg;
            bufferlen = nxtblen;
            msg = buffer;
        }
        
        memcpy((void*)&msg[msglen], data, datalen);
        msglen += datalen;
    }
}

void AesCmac::reset() {
    delete [] msg;
    bufferlen = 128, msglen = 0;
    msg = new unsigned char[bufferlen]();
}

// Pass K1, K2 arrays each of len 16 bytes
void AesCmac::_generateSubkey(unsigned char K1[16], unsigned char K2[16]) {
    unsigned char L[16] = {0};
    aes128Cipher.encryptBlock(L, const_Zero);
    unsigned char rhtmsb[16] = {0}; // MSB from byte to the right

    for (int i=1; i<16; ++i) rhtmsb[i-1] = L[i] >> 7;
    for (int i=0; i<16; ++i) K1[i] = (L[i] << 1) | rhtmsb[i];
    if (_MSB(L) != 0) _XOR_eq(K1, const_Rb);

    for (int i=1; i<16; ++i) rhtmsb[i-1] = K1[i] >> 7;
    for (int i=0; i<16; ++i) K2[i] = (K1[i] << 1) | rhtmsb[i];
    if (_MSB(K1) != 0) _XOR_eq(K2, const_Rb);
}

// Pass CMAC array of len 16 bytes
void AesCmac::finalize(unsigned char CMAC[16]) {
    int bidx = 0;
    unsigned char X[16] = {0};
    unsigned char Y[16] = {0};
    while (bidx < (int)msglen-16) {
        _XOR(Y, X, &msg[bidx]);
        aes128Cipher.encryptBlock(X,Y);
        bidx += 16;
    }

    // Special last block
    unsigned char K1[16] = {0};
    unsigned char K2[16] = {0};
    _generateSubkey(K1, K2);

    unsigned char mLast[16] = {0};
    if (msglen - bidx == 16) {
        _XOR(mLast, &msg[bidx], K1);
    } else {
        msg[msglen] = 0x80; // Padding bit
        _XOR(mLast, &msg[bidx], K2);
    }

    _XOR(Y, mLast, X);
    aes128Cipher.encryptBlock(CMAC,Y);
}
