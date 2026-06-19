#!/usr/bin/env bash
set -e

INSTALL_PREFIX="$HOME/.local"
BIN_DIR="${INSTALL_PREFIX}/bin"
APP_DIR="${INSTALL_PREFIX}/share/applications"
ICON_DIR="${INSTALL_PREFIX}/share/icons/hicolor/256x256/apps"

echo "=== Uninstalling OS DropZone ==="
rm -f "${BIN_DIR}/cross-os"
rm -f "${ICON_DIR}/cross-os.png"
rm -f "${APP_DIR}/cross-os.desktop"

if command -v update-desktop-database &> /dev/null; then
    update-desktop-database "${APP_DIR}" 2>/dev/null || true
fi

echo "OS DropZone has been successfully uninstalled."
