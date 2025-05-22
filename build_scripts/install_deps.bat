set -e

# check if arduino-cli is installed
if [ -z "$(command -v arduino-cli)" ]; then
    echo "arduino-cli not found. Please install it first: https://arduino.github.io/arduino-cli/latest/installation/"
    exit 1
fi

# check if esptool.py is installed
if [ -z "$(command -v esptool.py)" ]; then
    echo "esptool.py not found."
    exit 1
fi

# Install core
arduino-cli core install esp32:esp32

# Install deps
arduino-cli lib install ArduinoJson StreamUtils
arduino-cli config set library.enable_unsafe_install true
arduino-cli lib install --git-url https://github.com/ZinggJM/GxEPD2_4G.git
