#!/bin/bash

set -e

echo "Compiling Ashell..."

gcc *.c -o Ashell
echo "Done. Binary: ./Ashell"