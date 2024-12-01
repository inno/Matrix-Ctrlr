#!/bin/zsh

setopt errexit


# 0x7523 == vendor ID for CH340
device=$(arduino-cli board list --json | grep -B6 '0x7523' | grep address | cut -d: -f2 | tr -d '[", ]')

arduino-cli monitor --port $device --fqbn arduino:avr:mega --config 115200
