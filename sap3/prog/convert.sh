#!/bin/bash

# Get our basename and new extensions:
BFILE=${1%%.*}
NEWEXT=".COM"

cpp "$1" -imacros ../src/opcode.h -E -P | grep -v "^$" >> "$BFILE${NEWEXT}"
