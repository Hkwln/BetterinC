#!/bin/bash
# Manual build script for Game of Life Plymouth plugin
# Use this if not building with Plymouth's meson build system

set -e

PLUGIN_NAME="gameoflive"
SRC_DIR="../src"
PLUGIN_SRC="plugin.c"

# Check for Plymouth development files
if ! pkg-config --exists ply-splash-core ply-splash-graphics; then
    echo "Error: Plymouth development files not found!"
    echo "Install with: sudo pacman -S plymouth"
    exit 1
fi

echo "Building ${PLUGIN_NAME} Plymouth plugin..."

# Compile plugin
gcc -shared -fPIC -o ${PLUGIN_NAME}.so \
    ${PLUGIN_SRC} \
    ${SRC_DIR}/bitmap.c \
    ${SRC_DIR}/data.c \
    ${SRC_DIR}/draw.c \
    $(pkg-config --cflags --libs ply-splash-core ply-splash-graphics) \
    -I${SRC_DIR} \
    -Wall -Wextra 2>&1

echo "Build successful: ${PLUGIN_NAME}.so"
echo ""
echo "To install:"
echo "  sudo cp ${PLUGIN_NAME}.so /usr/lib/plymouth/renderers/"
echo "  sudo mkdir -p /usr/share/plymouth/themes/${PLUGIN_NAME}"
echo "  sudo cp ${PLUGIN_NAME}.plymouth /usr/share/plymouth/themes/${PLUGIN_NAME}/"
echo "  sudo plymouth-set-default-theme ${PLUGIN_NAME}"
echo "  sudo mkinitcpio -P"
echo ""
echo "To test (requires plymouth-x11):"
echo "  sudo plymouthd --debug --debug-file=/tmp/plymouth-debug.log"
echo "  sudo plymouth show-splash"
echo "  # Wait and watch..."
echo "  sudo plymouth quit"
echo "  cat /tmp/plymouth-debug.log"
