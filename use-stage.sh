#!/bin/bash
# Підставляє код відповідного етапу в src/ та перезбирає.
set -e
cd "$(dirname "$0")"
n=$(printf "%02d" "${1:?вкажіть номер етапу: ./use-stage.sh 3}")
[ -d "stages/$n/src" ] || { echo "Немає етапу $n"; exit 1; }
rm -rf src && cp -r "stages/$n/src" src
make
echo "--- Етап $n зібрано. Перевірте вручну: ./wish"
