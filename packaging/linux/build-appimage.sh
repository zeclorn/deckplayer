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

copy_if_exists() {
    local source_path="$1"
    local target_dir="$2"

    if [[ -e "${source_path}" ]]; then
        mkdir -p "${target_dir}"
        cp -a "${source_path}" "${target_dir}/"
    fi
}

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

cat > "${APPDIR}/usr/bin/qt.conf" <<'EOF'
[Paths]
Prefix=..
Plugins=plugins
Qml2Imports=qml
EOF

copy_if_exists /usr/lib/qt6/plugins/platforms "${APPDIR}/usr/plugins"
copy_if_exists /usr/lib/qt6/plugins/imageformats "${APPDIR}/usr/plugins"
copy_if_exists /usr/lib/qt6/plugins/platforminputcontexts "${APPDIR}/usr/plugins"
copy_if_exists /usr/lib/qt6/plugins/wayland-decoration-client "${APPDIR}/usr/plugins"
copy_if_exists /usr/lib/qt6/plugins/wayland-graphics-integration-client "${APPDIR}/usr/plugins"
copy_if_exists /usr/lib/qt6/plugins/wayland-shell-integration "${APPDIR}/usr/plugins"
copy_if_exists /usr/lib/qt6/qml/QtCore "${APPDIR}/usr/qml"
copy_if_exists /usr/lib/qt6/qml/QtQml "${APPDIR}/usr/qml"
copy_if_exists /usr/lib/qt6/qml/QtQuick "${APPDIR}/usr/qml"

APPIMAGE_EXTRACT_AND_RUN=1 NO_STRIP=1 LINUXDEPLOY_NO_STRIP=1 linuxdeploy \
    --appdir "${APPDIR}" \
    --desktop-file "${APPDIR}/steamdeckmediaplayer.desktop" \
    --icon-file "${APPDIR}/steamdeckmediaplayer.svg"

mkdir -p "${APPDIR}/usr/optional-libs"
mv "${APPDIR}"/usr/lib/libavformat.so* "${APPDIR}/usr/optional-libs/" 2>/dev/null || true

rm -f "${APPDIR}/AppRun"
cat > "${APPDIR}/AppRun" <<'EOF'
#!/usr/bin/env bash

set -euo pipefail

APPDIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUNDLED_AVFORMAT="${APPDIR}/usr/optional-libs/libavformat.so.62"

if [[ "${STEAMDECKMEDIAPLAYER_USE_BUNDLED_FFMPEG:-0}" == "1" ]] ||
    ! ldconfig -p 2>/dev/null | grep -q 'libavformat\.so\.62'; then
    export LD_PRELOAD="${BUNDLED_AVFORMAT}${LD_PRELOAD:+:${LD_PRELOAD}}"
fi

exec "${APPDIR}/usr/bin/steamdeckmediaplayer" "$@"
EOF
chmod +x "${APPDIR}/AppRun"

APPIMAGE_EXTRACT_AND_RUN=1 appimagetool "${APPDIR}"

mv -f "${ROOT_DIR}"/Steam_Deck_Media_Player-*.AppImage "${OUTPUT_DIR}/" 2>/dev/null || true

echo "AppImage output:"
find "${OUTPUT_DIR}" -maxdepth 1 -name "*.AppImage" -print
