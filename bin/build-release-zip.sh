#!/bin/bash

set -e

# Clean and rebuild the project
pio run -e lilygo213 -t clean
pio run -e lilygo213

# Create a zip of the required files in the root of the repo
cd .pio/build/lilygo213
zip ../../../release.zip firmware.bin bootloader.bin partitions.bin
