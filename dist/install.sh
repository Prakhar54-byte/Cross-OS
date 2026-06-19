#!/usr/bin/env bash
set -e

# Installer for OS DropZone (cross-os)
INSTALL_PREFIX="$HOME/.local"
BIN_DIR="${INSTALL_PREFIX}/bin"
APP_DIR="${INSTALL_PREFIX}/share/applications"
ICON_DIR="${INSTALL_PREFIX}/share/icons/hicolor/256x256/apps"

echo "=== Installing OS DropZone (cross-os) locally ==="
echo "Target directories:"
echo "  Binary:  ${BIN_DIR}"
echo "  Desktop: ${APP_DIR}"
echo "  Icon:    ${ICON_DIR}"

# Create directories if they don't exist
mkdir -p "${BIN_DIR}"
mkdir -p "${APP_DIR}"
mkdir -p "${ICON_DIR}"

# Copy files
cp -f cross-os "${BIN_DIR}/"
chmod +x "${BIN_DIR}/cross-os"
cp -f icon.png "${ICON_DIR}/cross-os.png"

# Setup Desktop entry with absolute paths
cp -f cross-os.desktop "${APP_DIR}/"
sed -i "s|Exec=cross-os|Exec=${BIN_DIR}/cross-os|g" "${APP_DIR}/cross-os.desktop"
sed -i "s|Icon=cross-os|Icon=${ICON_DIR}/cross-os.png|g" "${APP_DIR}/cross-os.desktop"
chmod +x "${APP_DIR}/cross-os.desktop"

# Try to update desktop database
if command -v update-desktop-database &> /dev/null; then
    update-desktop-database "${APP_DIR}" 2>/dev/null || true
fi

echo "==========================================="
echo "Installation complete!"
echo "You can launch 'OS DropZone' from your application menu,"
echo "or run it from the command line using: ~/.local/bin/cross-os"
echo "==========================================="
