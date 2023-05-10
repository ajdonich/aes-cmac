#!/usr/bin/env bash
set -e

echo
echo "=="
echo "== Cleaning aes-cmac"
echo "=="
echo

pushd "$(dirname "$0")/.." > /dev/null
rm -rf cmake-build/
popd > /dev/null
