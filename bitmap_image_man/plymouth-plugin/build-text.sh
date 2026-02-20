#!/bin/bash
# Build script for TEXT-based Game of Life Plymouth plugin
# Much simpler than pixel version!

set -e

PLUGIN_NAME="gameoflive"
SRC_DIR="../src"
PLUGIN_SRC="plugin-text.c"

# Check for Plymouth development files
if ! pkg-config --exists ply-splash-core; then
    echo "Error: Plymouth development files not found!"
    echo "Install with: sudo pacman -S plymouth"
    exit 1
fi

echo "Building TEXT-based ${PLUGIN_NAME} Plymouth plugin..."

# Compile plugin (simpler - no graphics libs needed!)
gcc -shared -fPIC -o ${PLUGIN_NAME}.so \
    ${PLUGIN_SRC} \
    ${SRC_DIR}/bitmap.c \
    ${SRC_DIR}/data.c \
    ${SRC_DIR}/draw.c \
    $(pkg-config --cflags --libs ply-splash-core) \
    -I${SRC_DIR} \
    -Wall -Wextra 2>&1

echo "Build successful: ${PLUGIN_NAME}.so"
echo ""
echo "To install:"
echo "  sudo cp ${PLUGIN_NAME}.so /usr/lib/plymouth/"
echo "  sudo mkdir -p /usr/share/plymouth/themes/${PLUGIN_NAME}"
echo "  sudo cp ${PLUGIN_NAME}.plymouth /usr/share/plymouth/themes/${PLUGIN_NAME}/"
echo "  sudo plymouth-set-default-theme ${PLUGIN_NAME}"
echo "  sudo mkinitcpio -P"
