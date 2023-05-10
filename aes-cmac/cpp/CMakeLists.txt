cmake_minimum_required(VERSION 3.15)

# =========
# Configure
# =========

# Set project name and lang: C++
project(aes-mac LANGUAGES CXX)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Fetch external deps
include(FetchContent)
FetchContent_Declare(arduinolibs 
    GIT_REPOSITORY https://github.com/rweather/arduinolibs.git
    GIT_TAG        1e13ef7b244caaa1b9151d5e41c4cdb029f4ea84 # latest commit as of 05/10/2023
)
FetchContent_Declare(googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.13.0
)
FetchContent_MakeAvailable(arduinolibs googletest)

# =====
# Build
# =====

# Build arduinocrypto library
set(ARDUINO_SRC "../../cmake-build/_deps/arduinolibs-src")
set(CRYTPO_CPP
    "${ARDUINO_SRC}/libraries/CryptoLW/src/Acorn128.cpp"
    "${ARDUINO_SRC}/libraries/Crypto/AES128.cpp"
    "${ARDUINO_SRC}/libraries/Crypto/AES192.cpp"
    "${ARDUINO_SRC}/libraries/Crypto/AES256.cpp"
    "${ARDUINO_SRC}/libraries/Crypto/AESCommon.cpp"
    "${ARDUINO_SRC}/libraries/CryptoLW/src/Ascon128.cpp"
    "${ARDUINO_SRC}/libraries/Crypto/AuthenticatedCipher.cpp"
    "${ARDUINO_SRC}/libraries/Crypto/BigNumberUtil.cpp"
    "${ARDUINO_SRC}/libraries/Crypto/BLAKE2b.cpp"
    "${ARDUINO_SRC}/libraries/Crypto/BLAKE2s.cpp"
    "${ARDUINO_SRC}/libraries/Crypto/BlockCipher.cpp"
    "${ARDUINO_SRC}/libraries/CryptoLegacy/src/CBC.cpp"
    "${ARDUINO_SRC}/libraries/CryptoLegacy/src/CFB.cpp"
    "${ARDUINO_SRC}/libraries/Crypto/ChaCha.cpp"
    "${ARDUINO_SRC}/libraries/Crypto/ChaChaPoly.cpp"
    "${ARDUINO_SRC}/libraries/Crypto/Cipher.cpp"
    "${ARDUINO_SRC}/libraries/Crypto/Crypto.cpp"
    "${ARDUINO_SRC}/libraries/Crypto/CTR.cpp"
    "${ARDUINO_SRC}/libraries/Crypto/Curve25519.cpp"
    "${ARDUINO_SRC}/libraries/Crypto/EAX.cpp"
    "${ARDUINO_SRC}/libraries/Crypto/Ed25519.cpp"
    "${ARDUINO_SRC}/libraries/Crypto/GCM.cpp"
    "${ARDUINO_SRC}/libraries/Crypto/GF128.cpp"
    "${ARDUINO_SRC}/libraries/Crypto/GHASH.cpp"
    "${ARDUINO_SRC}/libraries/Crypto/Hash.cpp"
    "${ARDUINO_SRC}/libraries/Crypto/HKDF.cpp"
    "${ARDUINO_SRC}/libraries/Crypto/KeccakCore.cpp"
    "${ARDUINO_SRC}/libraries/NewHope/NewHope.cpp"
    "${ARDUINO_SRC}/libraries/Crypto/NoiseSource.cpp"
    "${ARDUINO_SRC}/libraries/CryptoLegacy/src/OFB.cpp"
    "${ARDUINO_SRC}/libraries/Crypto/OMAC.cpp"
    "${ARDUINO_SRC}/libraries/Crypto/P521.cpp"
    "${ARDUINO_SRC}/libraries/Crypto/Poly1305.cpp"
    "${ARDUINO_SRC}/host/Crypto/RNG_host.cpp"
    "${ARDUINO_SRC}/libraries/CryptoLegacy/src/SHA1.cpp"
    "${ARDUINO_SRC}/libraries/Crypto/SHA224.cpp"
    "${ARDUINO_SRC}/libraries/Crypto/SHA256.cpp"
    "${ARDUINO_SRC}/libraries/Crypto/SHA384.cpp"
    "${ARDUINO_SRC}/libraries/Crypto/SHA3.cpp"
    "${ARDUINO_SRC}/libraries/Crypto/SHA512.cpp"
    "${ARDUINO_SRC}/libraries/Crypto/SHAKE.cpp"
    "${ARDUINO_SRC}/libraries/CryptoLW/src/Speck.cpp"
    "${ARDUINO_SRC}/libraries/CryptoLW/src/SpeckSmall.cpp"
    "${ARDUINO_SRC}/libraries/CryptoLW/src/SpeckTiny.cpp"
    "${ARDUINO_SRC}/libraries/Crypto/XOF.cpp"
    "${ARDUINO_SRC}/libraries/Crypto/XTS.cpp"
    AesCmac.cpp)
add_library(arduinocrypto SHARED ${CRYTPO_CPP})
target_include_directories(arduinocrypto PUBLIC 
    "${ARDUINO_SRC}/host/emulation"
    "${ARDUINO_SRC}/host/emulation/avr"
    "${ARDUINO_SRC}/libraries/Crypto"
    "${ARDUINO_SRC}/libraries/Crypto/utility"
    "${ARDUINO_SRC}/libraries/CryptoLW/src"
    "${ARDUINO_SRC}/libraries/CryptoLegacy/src"
    "${ARDUINO_SRC}/libraries/NewHope"
    "${ARDUINO_SRC}/libraries/RingOscillatorNoiseSource"
    "${ARDUINO_SRC}/libraries/TransistorNoiseSource")

# Build AesCmac_test google test executable
enable_testing()
add_executable(AesCmac_test "../../test/AesCmac_test.cpp")
target_link_libraries(AesCmac_test PRIVATE arduinocrypto GTest::gtest_main)
target_include_directories(AesCmac_test PUBLIC ".")

include(GoogleTest)
gtest_discover_tests(AesCmac_test)

# Build fittransformer_so cpython module
# pybind11_add_module(fittransformer_so fittransformer.cc fittransformer_so.cc)
# target_compile_definitions(fittransformer_so PRIVATE -DPYBIND11_PRINT_PYSTDOUT)
# target_link_libraries(fittransformer_so PRIVATE arrow_shared parquet_shared
#     Boost::filesystem pybind11::module pybind11::lto fitsdk)

# =======
# Install
# =======
