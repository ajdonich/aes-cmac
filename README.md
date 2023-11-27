# AES-CMAC
C implementation of AES-CMAC algorithm ([RFC4493](https://www.rfc-editor.org/rfc/rfc4493)) for ESP32/Arduino platforms. In particular this algorithm is required by [LoRaWAN Network Protocol Specifications](https://resources.lora-alliance.org/technical-specifications/ts001-1-0-4-lorawan-l2-1-0-4-specification) to generate and verify Message Integrity Codes. This repo also contains a pybind11 binding/module to access this AES-CMAC implementation from Python scripts. It requires Python 3.7 or greater and NumPy.

This project contains a build/runtime dependency on the [Arduino Cryptography Library](https://github.com/rweather/arduinolibs). If building from repo source, the top-level Makefile can be used, but CMake is also required and will fetch and build the arduinolibs automatically. For an ESP32 project, arduinolibs is available as a [PlatformIO](https://platformio.org/) library. Other than this library only [AesCmac.h](https://github.com/ajdonich/aes-cmac/blob/main/aes-cmac/AesCmac.h) and [AesCmac.cpp](https://github.com/ajdonich/aes-cmac/blob/main/aes-cmac/AesCmac.cpp) are needed and can be easily dropped into an ESP32 project file hierarchy directly.  


## Quickstart

C++ Example:
```C++
#include <iostream>
#include "AesCmac.h"

std::string toString(unsigned char *buf, size_t len=16) {
    size_t maxbytes = len*2 + ((int)len/4) + 1;
    char cstr[maxbytes];
    
    for(int i = 0, j = 0; i < len; i++) {
        if (i > 0 && (i%4) == 0) { 
            std::snprintf(&cstr[j], maxbytes, "%s", " ");
            j += 1;
        }
        std::snprintf(&cstr[j], maxbytes, "%02x", buf[i]);
        j += 2;
    }
    return std::string(cstr);
}

int main() {
    unsigned char K[16] = {
        0x2b,0x7e,0x15,0x16,0x28,0xae,0xd2,0xa6,
        0xab,0xf7,0x15,0x88,0x09,0xcf,0x4f,0x3c };

    unsigned char M[] = {
        0x6b,0xc1,0xbe,0xe2,0x2e,0x40,0x9f,0x96,
        0xe9,0x3d,0x7e,0x11,0x73,0x93,0x17,0x2a,
        0xae,0x2d,0x8a,0x57,0x1e,0x03,0xac,0x9c,
        0x9e,0xb7,0x6f,0xac,0x45,0xaf,0x8e,0x51,
        0x30,0xc8,0x1c,0x46,0xa3,0x5c,0xe4,0x11 };

    unsigned char CMAC[16];

    AesCmac aes(K);
    aes.append(M, sizeof(M));
    aes.finalize(CMAC);

    std::cout << toString(CMAC, sizeof(CMAC)) << std::endl;
}
```

Python Example:
```Python
from pyaescmac import aes128_cmac
import numpy as np

def hex_pack(hexstring):
    hxstr = hexstring.replace(' ', '').replace('\n', '')
    ivals = [int(hxstr[i:i+2], 16) for i in range(0, len(hxstr), 2)]
    return np.array(ivals, dtype=np.ubyte)

def np_unpack(nparray):
    result = ''
    for i, ival in enumerate(nparray):
        if i>0 and i%4 == 0: result += ' '
        result += f"{ival:02x}"
    return result

if __name__ == "__main__":
    K = '2b7e1516 28aed2a6 abf71588 09cf4f3c'
    M = '''6bc1bee2 2e409f96 e93d7e11 7393172a
           ae2d8a57 1e03ac9c 9eb76fac 45af8e51
           30c81c46 a35ce411'''
    CMAC = aes128_cmac(hex_pack(K), hex_pack(M))
    print(np_unpack(CMAC))
```

Output (for either C++ or Python example):
```
dfa66747 de9ae630 30ca3261 1497c827
```

For further code examples see test files: [AesCmac_test.cpp](https://github.com/ajdonich/aes-cmac/blob/main/test/AesCmac_test.cpp) and [PyAesCmac_test.py](https://github.com/ajdonich/aes-cmac/blob/main/test/PyAesCmac_test.py).