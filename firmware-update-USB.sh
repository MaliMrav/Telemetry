#!/bin/zsh

set -e

PROJECT_DIR="/Users/vladimir/Documents/GitHub/telemetry"
ESPTOOL="$HOME/.platformio/penv/bin/esptool"

BAUD="460800"

# Auto-detect ESP USB-serial adapter (CH340, CP210x, FTDI)
PORTS=(/dev/cu.usbserial-*(N) /dev/cu.wchusbserial-*(N) /dev/cu.SLAB_USBtoUART*(N))

if [[ ${#PORTS[@]} -eq 0 ]]; then
    echo "ERROR: No USB-serial device found. Check the cable and driver."
    exit 1
elif [[ ${#PORTS[@]} -gt 1 ]]; then
    echo "ERROR: Multiple USB-serial devices found:"
    printf '       %s\n' "${PORTS[@]}"
    echo "Unplug all but the target device and retry."
    exit 1
fi

PORT="${PORTS[1]}"

FIRMWARE="$PROJECT_DIR/.pio/build/ESP8266/firmware.bin"

echo "=== Telemetry Firmware USB Update ==="
echo
echo "Port:    $PORT"
echo "Source:  $FIRMWARE"
echo

if [[ ! -f "$FIRMWARE" ]]; then
    echo "ERROR: Firmware image not found:"
    echo "       $FIRMWARE"
    echo
    echo "Build the firmware first with:"
    echo "    pio run"
    exit 1
fi

cd "$PROJECT_DIR"

python3 "$ESPTOOL" \
    --port "$PORT" \
    --baud "$BAUD" \
    write-flash 0x0 \
    "$FIRMWARE"

echo
echo "=== Firmware update complete ==="
