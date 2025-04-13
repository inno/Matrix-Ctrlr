VERSION := "9.99"
YEAR := $(shell date -I | cut -d- -f1)
CPPFLAGS := -DVERSION=\"${VERSION}\" -DYEAR=\"${YEAR}\"
FQBN := arduino:avr:mega
BUILD_PROPERTY := compiler.cpp.extra_flags="$(CPPFLAGS)"
DEVICE := $(shell \
		  arduino-cli board list --json \
		  | grep -B6 '0x7523' \
		  | grep address \
		  | cut -d: -f2 \
		  | tr -d '[", ]' \
		)

.SILENT: _require_device

all: core build upload

core:
	git submodule update
	arduino-cli core update-index
	arduino-cli core install arduino:avr

build: main/*.ino main/*.h
	arduino-cli compile \
    	--fqbn $(FQBN) \
	    --libraries lib \
    	--build-property $(BUILD_PROPERTY) \
	    --export-binaries \
    	main

_require_device:
	# 0x7523 == vendor ID for CH340
	echo "Attempting to update ${DEVICE}..."
	if [ "$(DEVICE)" == "" ]; then \
		echo "No Matrix-Ctrlr found!"; \
		exit -1; \
	fi

upload: _require_device
	arduino-cli upload --fqbn arduino:avr:mega --port $device --verbose main


monitor: _require_device
	arduino-cli monitor --port $device --fqbn arduino:avr:mega --config 115200
