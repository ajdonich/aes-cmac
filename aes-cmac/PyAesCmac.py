
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
    print("\n--------------------------------------------------")
    K = '2b7e1516 28aed2a6 abf71588 09cf4f3c'
    print(f'K     {K}\n')

    M = ''
    CMAC = np_unpack(aes128_cmac(hex_pack(K), hex_pack(M)))
    print('Example 1: len = 0')
    print(f'M     <empty string>')
    print(f'CMAC  {CMAC}')
    print(f'{"PASSED" if CMAC == "bb1d6929 e9593728 7fa37d12 9b756746" else "FAILED"}')
    print("--------------------------------------------------\n")

    M = '6bc1bee2 2e409f96 e93d7e11 7393172a'
    CMAC = np_unpack(aes128_cmac(hex_pack(K), hex_pack(M)))
    print('Example 2: len = 16')
    print(f'M     {M}')
    print(f'CMAC  {CMAC}')
    print(f'{"PASSED" if CMAC == "070a16b4 6b4d4144 f79bdd9d d04a287c" else "FAILED"}')
    print("--------------------------------------------------\n")

    M = '''6bc1bee2 2e409f96 e93d7e11 7393172a
      ae2d8a57 1e03ac9c 9eb76fac 45af8e51
      30c81c46 a35ce411'''
    CMAC = np_unpack(aes128_cmac(hex_pack(K), hex_pack(M)))
    print('Example 3: len = 40')
    print(f'M     {M}')
    print(f'CMAC  {CMAC}')
    print(f'{"PASSED" if CMAC == "dfa66747 de9ae630 30ca3261 1497c827" else "FAILED"}')
    print("--------------------------------------------------\n")


    M = '''6bc1bee2 2e409f96 e93d7e11 7393172a
      ae2d8a57 1e03ac9c 9eb76fac 45af8e51
      30c81c46 a35ce411 e5fbc119 1a0a52ef
      f69f2445 df4f9b17 ad2b417b e66c3710'''
    CMAC = np_unpack(aes128_cmac(hex_pack(K), hex_pack(M)))
    print('Example 4: len = 64')
    print(f'M     {M}')
    print(f'CMAC  {CMAC}')
    print(f'{"PASSED" if CMAC == "51f0bebf 7e3b9d92 fc497417 79363cfe" else "FAILED"}')
    print("--------------------------------------------------\n")

