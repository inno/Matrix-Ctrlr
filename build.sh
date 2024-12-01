#!/bin/zsh

setopt errexit

git submodule update
arduino-cli core update-index
arduino-cli core install arduino:avr

VERSION="9.99"

echo "Compiling ${VERSION}..."
arduino-cli compile \
    --fqbn arduino:avr:mega \
    --libraries lib \
    --build-property compiler.cpp.extra_flags="'-DVERSION=\"${VERSION}\"'" \
    --export-binaries \
    main



# 0x7523 == vendor ID for CH340
device=$(arduino-cli board list --json | grep -B6 '0x7523' | grep address | cut -d: -f2 | tr -d '[", ]')
echo Attempting to update ${device}...

## Send data to device
arduino-cli upload --fqbn arduino:avr:mega --port $device --verbose main
