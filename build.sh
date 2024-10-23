
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


## Send data to device
# arduino-cli upload --fqbn arduino:avr:mega --port /dev/cu.usbserial-110 --verbose main
