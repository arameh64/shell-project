#!/bin/bash

set -e

echo "Compiling Ashell..."

gcc -Wall -Wextra -Werror *.c -o Ashell
echo "Done. Binary: ./Ashell"