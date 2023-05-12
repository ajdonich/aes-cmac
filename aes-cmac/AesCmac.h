#include <cstring>
#include <AES.h>

class AesCmac 
{
public:
    // Init with 16-byte/AES128 hash key
    AesCmac(unsigned char key[16]);
    ~AesCmac();

    // Appends data to the internal message buffer for hash
    void append(const unsigned char *data, size_t datalen);
 
    // Hash internal message buffer and put result in CMAC array
    void finalize(unsigned char CMAC[16]); 

    // Reset/clear message buffer
    void reset(); 

    // Internal helper functions
    unsigned char _MSB(unsigned char *barray);
    void _XOR(unsigned char *result, const unsigned char *lhs, const unsigned char *rhs, size_t len=16);
    void _XOR_eq(unsigned char *result, const unsigned char *rhs, size_t len=16);
    void _generateSubkey(unsigned char K1[16], unsigned char K2[16]);

private:
    AES128 aes128Cipher;
    unsigned char K[16];
    unsigned char *msg;
    size_t bufferlen;
    size_t msglen;
};

inline unsigned char AesCmac::_MSB(unsigned char *barray) { 
    return barray[0] >> 7;
}

inline void AesCmac::_XOR(unsigned char *result, const unsigned char *lhs, const unsigned char *rhs, size_t len) {
    for (int i=0; i<len; ++i) result[i] = lhs[i] ^ rhs[i];
}

inline void AesCmac::_XOR_eq(unsigned char *result, const unsigned char *rhs, size_t len) {
    for (int i=0; i<len; ++i) result[i] ^= rhs[i];
}
