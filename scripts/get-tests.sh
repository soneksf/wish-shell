#!/bin/bash
# Перезавантажує офіційні тести OSTEP (якщо треба оновити).
set -e
cd "$(dirname "$0")/.."
TMP=$(mktemp -d)
git clone --depth 1 https://github.com/remzi-arpacidusseau/ostep-projects.git "$TMP"
rm -rf tests tester
cp -r "$TMP/processes-shell/tests" ./tests
cp -r "$TMP/tester" ./tester
# офіційний скрипт шукає ../tester — правимо на локальний каталог
sed 's|\.\./tester/run-tests.sh|./tester/run-tests.sh|' "$TMP/processes-shell/test-wish.sh" > ./test-wish.sh
chmod +x ./test-wish.sh ./tester/run-tests.sh
rm -rf "$TMP"
echo "Готово. Запуск: make && ./test-wish.sh"
