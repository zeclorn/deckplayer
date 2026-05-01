#!/usr/bin/env bash

set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"
BUILD_DIR="${ROOT_DIR}/build-linux"
APPDIR="${ROOT_DIR}/AppDir"
OUTPUT_DIR="${ROOT_DIR}/dist"

require_cmd() {
    if ! command -v "$1" >/dev/null 2>&1; then
        echo "Missing required command: $1" >&2
        exit 1
    fi
}

require_cmd cmake
require_cmd linuxdeploy
require_cmd appimagetool
require_cmd pkg-config

if [[ "$(uname -s)" != "Linux" ]]; then
    echo "AppImage packaging must be run on Linux." >&2
    exit 1
fi

rm -rf "${BUILD_DIR}" "${APPDIR}"
mkdir -p "${OUTPUT_DIR}"

cmake -S "${ROOT_DIR}" -B "${BUILD_DIR}" \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INSTALL_PREFIX=/usr

cmake --build "${BUILD_DIR}" --config Release
cmake --install "${BUILD_DIR}" --prefix "${APPDIR}/usr"

cp "${ROOT_DIR}/packaging/linux/steamdeckmediaplayer.desktop" "${APPDIR}/"
cp "${ROOT_DIR}/assets/icons/steamdeckmediaplayer.svg" "${APPDIR}/steamdeckmediaplayer.svg"

APPIMAGE_EXTRACT_AND_RUN=1 NO_STRIP=1 LINUXDEPLOY_NO_STRIP=1 linuxdeploy \
    --appdir "${APPDIR}" \
    --desktop-file "${APPDIR}/steamdeckmediaplayer.desktop" \
    --icon-file "${APPDIR}/steamdeckmediaplayer.svg" \
    --output appimage

mv -f "${ROOT_DIR}"/Steam_Deck_Media_Player-*.AppImage "${OUTPUT_DIR}/" 2>/dev/null || true

echo "AppImage output:"
find "${OUTPUT_DIR}" -maxdepth 1 -name "*.AppImage" -print
