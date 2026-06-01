#!/usr/bin/env bash

set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"
DESTDIR="${DESTDIR:?DESTDIR must be set by rpmbuild or OBS}"
APPIMAGE_INSTALL_DIR="${APPIMAGE_INSTALL_DIR:-${DESTDIR}/usr/libexec/deckplayer}"
OUTPUT_DIR="${OUTPUT_DIR:-${ROOT_DIR}/dist-obs}"
BUILD_DIR="${BUILD_DIR:-${ROOT_DIR}/build-obs-appimage}"
APPDIR="${APPDIR:-${ROOT_DIR}/AppDir-obs}"

export OUTPUT_DIR
export BUILD_DIR
export APPDIR

"${ROOT_DIR}/packaging/linux/build-appimage.sh"

appimage_path="$(find "${OUTPUT_DIR}" -maxdepth 1 -name "*.AppImage" -print -quit)"
if [[ -z "${appimage_path}" ]]; then
    echo "Expected an AppImage in ${OUTPUT_DIR}, but none was produced." >&2
    exit 1
fi

mkdir -p "${APPIMAGE_INSTALL_DIR}"
install -m 0755 "${appimage_path}" "${APPIMAGE_INSTALL_DIR}/"
