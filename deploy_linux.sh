#!/usr/bin/env bash
set -e

# OS DropZone - Linux Packaging Script
# This script builds the application in Release mode and bundles it into a tarball with an installer.

PROJECT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="${PROJECT_DIR}/build-release"
DIST_DIR="${PROJECT_DIR}/dist"
TARBALL_NAME="cross-os-linux.tar.gz"

echo "=== OS DropZone Linux Deployment Build ==="
echo "Project Directory: ${PROJECT_DIR}"
echo "Build Directory: ${BUILD_DIR}"
echo "Distribution Directory: ${DIST_DIR}"
echo "========================================="

# 1. Clean / Create Build Directory
if [ -d "${BUILD_DIR}" ]; then
    echo "Cleaning old build directory..."
    rm -rf "${BUILD_DIR}"
fi
mkdir -p "${BUILD_DIR}"

# 2. Configure and Build with CMake (Release mode)
cd "${BUILD_DIR}"
echo "Configuring project in Release mode..."
cmake -DCMAKE_BUILD_TYPE=Release ..

echo "Building project..."
cmake --build . --config Release

# 3. Create Dist Folder
if [ -d "${DIST_DIR}" ]; then
    rm -rf "${DIST_DIR}"
fi
mkdir -p "${DIST_DIR}"

echo "Gathering release files..."
# Copy Binary
cp "${BUILD_DIR}/cross-os" "${DIST_DIR}/"
# Copy Icons and Desktop entry
cp "${PROJECT_DIR}/icon.png" "${DIST_DIR}/"
cp "${PROJECT_DIR}/cross-os.desktop" "${DIST_DIR}/"

# 4. Create install.sh script inside dist
cat << 'EOF' > "${DIST_DIR}/install.sh"
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
EOF

chmod +x "${DIST_DIR}/install.sh"

# 5. Create uninstall.sh script inside dist
cat << 'EOF' > "${DIST_DIR}/uninstall.sh"
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
EOF

chmod +x "${DIST_DIR}/uninstall.sh"

# 6. Create Tarball package
echo "Packaging into ${TARBALL_NAME}..."
cd "${PROJECT_DIR}"
tar -czf "${TARBALL_NAME}" -C "${DIST_DIR}" .

echo "========================================="
echo "SUCCESS! Created ${TARBALL_NAME}"
echo "To install on this system, you can run:"
echo "  cd ${DIST_DIR} && ./install.sh"
echo "========================================="
