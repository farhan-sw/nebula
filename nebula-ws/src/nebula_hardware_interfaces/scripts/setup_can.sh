#!/bin/bash

# Vendor ID and Product ID of the device
VENDOR_ID="16d0"
PRODUCT_ID="117e"

# Iterate over all ttyACM devices
for DEV in /dev/ttyACM*; do
    if [ -e "$DEV" ]; then
        UDEV_INFO=$(udevadm info --query=all --name=$DEV)

        # Check if the device matches the Vendor ID and Product ID
        if echo "$UDEV_INFO" | grep -q "ID_VENDOR_ID=$VENDOR_ID" && echo "$UDEV_INFO" | grep -q "ID_MODEL_ID=$PRODUCT_ID"; then
            TTY_DEVICE=$DEV
            break
        fi
    fi
done

if [ -z "$TTY_DEVICE" ]; then
    echo "No ttyACM device found for Vendor ID $VENDOR_ID and Product ID $PRODUCT_ID."
    exit 1
fi

echo "Using device: $TTY_DEVICE"

# Map ttyACM* to can0 and set the bit rate to 500K
sudo slcand -o -c -s6 $TTY_DEVICE can0

# Enable CAN
sudo ifconfig can0 up

# Configure the transmission data buffer length
sudo ifconfig can0 txqueuelen 1000

echo "CAN setup complete."

