#!/bin/bash

set -e

CC=gcc
CFLAGS="-Wall -Wextra -Werror -std=c99"
OUT="ashell"

echo "[*] Cleaning old build..."
rm -f *.o "$OUT"

echo "[*] Compiling..."

for file in *.c; do
    echo "  -> $file"
    $CC $CFLAGS -c "$file"
done

echo "[*] Linking..."
$CC $CFLAGS *.o -o "$OUT"

echo "[+] Build complete: ./$OUT"
