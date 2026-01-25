#!/bin/bash

BUILD_DIR="cmake-build-debug"
TARGET_DIR="python/masterchip8"

mkdir -p "$TARGET_DIR"

LIB_FILE=$(find "$BUILD_DIR" -name "masterchip8_py*.so" | head -n 1)

if [ -z "$LIB_FILE" ]; then
    echo "Error: Could not find masterchip8_py*.so in $BUILD_DIR"
    echo "Did you build the project?"
    exit 1
fi

echo "Found library: $LIB_FILE"
echo "Copying to $TARGET_DIR..."

cp "$LIB_FILE" "$TARGET_DIR/"

echo "Done!"
