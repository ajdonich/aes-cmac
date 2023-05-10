#!/usr/bin/env bash
set -e

AES_CMAC_ROOT=$(cd "$(dirname "$0")/.."; pwd;)
: ${CRYPTO_LIB:=${AES_CMAC_ROOT}/arduinolibs}
pushd ${AES_CMAC_ROOT} > /dev/null

echo
echo "=="
echo "== Building libarduinocrypto.dylib"
echo "=="
echo

cmake -Saes-cmac/cpp -Bcmake-build
cmake --build cmake-build

echo
echo "=="
echo "== Testing AES-CMAC: AesCmac_test"
echo "=="
echo

./cmake-build/AesCmac_test
popd > /dev/null
