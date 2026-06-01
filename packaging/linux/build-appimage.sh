#!/usr/bin/env bash

set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"
BUILD_DIR="${BUILD_DIR:-${ROOT_DIR}/build-linux}"
APPDIR="${APPDIR:-${ROOT_DIR}/AppDir}"
OUTPUT_DIR="${OUTPUT_DIR:-${ROOT_DIR}/dist}"
STEAMOS_MAX_GLIBC="${STEAMOS_MAX_GLIBC:-2.41}"
LINUXDEPLOY_BIN="${LINUXDEPLOY_BIN:-linuxdeploy}"
APPIMAGETOOL_BIN="${APPIMAGETOOL_BIN:-appimagetool}"

find_cmd() {
    local candidate
    for candidate in "$@"; do
        if command -v "${candidate}" >/dev/null 2>&1; then
            printf '%s\n' "${candidate}"
            return 0
        fi
    done
    return 1
}

require_cmd() {
    if ! command -v "$1" >/dev/null 2>&1; then
        echo "Missing required command: $1" >&2
        exit 1
    fi
}

copy_glob_if_exists() {
    local source_glob="$1"
    local target_dir="$2"
    local source_path

    shopt -s nullglob
    for source_path in ${source_glob}; do
        mkdir -p "${target_dir}"
        cp -a "${source_path}" "${target_dir}/"
    done
    shopt -u nullglob
}

first_existing_path() {
    local candidate
    for candidate in "$@"; do
        if [[ -e "${candidate}" ]]; then
            printf '%s\n' "${candidate}"
            return 0
        fi
    done
    return 1
}

require_cmd cmake
require_cmd "${LINUXDEPLOY_BIN}"
require_cmd "${APPIMAGETOOL_BIN}"
require_cmd pkg-config
require_cmd readelf

copy_if_exists() {
    local source_path="$1"
    local target_dir="$2"

    if [[ -e "${source_path}" ]]; then
        mkdir -p "${target_dir}"
        cp -a "${source_path}" "${target_dir}/"
    fi
}

copy_qt_library() {
    local library_name="$1"

    copy_glob_if_exists "${QT_LIBRARY_DIR}/${library_name}.so*" "${APPDIR}/usr/lib"
    copy_glob_if_exists "/usr/lib*/${library_name}.so*" "${APPDIR}/usr/lib"
    copy_glob_if_exists "/usr/lib*/*/${library_name}.so*" "${APPDIR}/usr/lib"
}

version_gt() {
    [[ "$(printf '%s\n%s\n' "$1" "$2" | sort -V | tail -n 1)" == "$1" && "$1" != "$2" ]]
}

host_glibc_version() {
    ldd --version | sed -n '1s/.* //p'
}

if [[ "$(uname -s)" != "Linux" ]]; then
    echo "AppImage packaging must be run on Linux." >&2
    exit 1
fi

HOST_GLIBC="$(host_glibc_version)"
if version_gt "${HOST_GLIBC}" "${STEAMOS_MAX_GLIBC}" &&
    [[ "${STEAMDECKMEDIAPLAYER_ALLOW_NEW_GLIBC:-0}" != "1" ]]; then
    cat >&2 <<EOF
This build host uses glibc ${HOST_GLIBC}, which is newer than the SteamOS
compatibility target ${STEAMOS_MAX_GLIBC}.

Build this AppImage on SteamOS, or in a Linux container/VM with glibc
${STEAMOS_MAX_GLIBC} or older. Newer bundled libraries can require symbols
that SteamOS does not provide, causing runtime errors such as:

    GLIBC_2.43 not found

Set STEAMDECKMEDIAPLAYER_ALLOW_NEW_GLIBC=1 only for local, non-SteamOS test
builds.
EOF
    exit 1
fi

QTPATHS_BIN="${QTPATHS_BIN:-$(find_cmd qtpaths6 qtpaths || true)}"
QT_PLUGIN_DIR="${QT_PLUGIN_DIR:-}"
QT_QML_DIR="${QT_QML_DIR:-${QML2_IMPORT_PATH:-}}"
QT_LIBRARY_DIR="${QT_LIBRARY_DIR:-}"

if [[ -z "${QT_PLUGIN_DIR}" && -n "${QTPATHS_BIN}" ]]; then
    QT_PLUGIN_DIR="$("${QTPATHS_BIN}" --query QT_INSTALL_PLUGINS 2>/dev/null || true)"
fi

if [[ -z "${QT_QML_DIR}" && -n "${QTPATHS_BIN}" ]]; then
    QT_QML_DIR="$("${QTPATHS_BIN}" --query QT_INSTALL_QML 2>/dev/null || true)"
fi

if [[ -z "${QT_LIBRARY_DIR}" && -n "${QTPATHS_BIN}" ]]; then
    QT_LIBRARY_DIR="$("${QTPATHS_BIN}" --query QT_INSTALL_LIBS 2>/dev/null || true)"
fi

QT_PLUGIN_DIR="${QT_PLUGIN_DIR:-$(first_existing_path \
    /usr/lib64/qt6/plugins \
    /usr/lib/qt6/plugins \
    /usr/lib/x86_64-linux-gnu/qt6/plugins || true)}"
QT_QML_DIR="${QT_QML_DIR:-$(first_existing_path \
    /usr/lib64/qt6/qml \
    /usr/lib/qt6/qml \
    /usr/lib/x86_64-linux-gnu/qt6/qml || true)}"
QT_LIBRARY_DIR="${QT_LIBRARY_DIR:-$(first_existing_path \
    /usr/lib64 \
    /usr/lib \
    /usr/lib/x86_64-linux-gnu || true)}"
MUJS_LIB_GLOB="${MUJS_LIB_GLOB:-$(first_existing_path \
    /usr/lib64/libmujs.so* \
    /usr/lib/libmujs.so* \
    /usr/lib/x86_64-linux-gnu/libmujs.so* || true)}"

rm -rf "${BUILD_DIR}" "${APPDIR}"
mkdir -p "${OUTPUT_DIR}"

cmake -S "${ROOT_DIR}" -B "${BUILD_DIR}" \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INSTALL_PREFIX=/usr

cmake --build "${BUILD_DIR}" --config Release
cmake --install "${BUILD_DIR}" --prefix "${APPDIR}/usr"

cp "${ROOT_DIR}/packaging/linux/deckplayer.desktop" "${APPDIR}/"
cp "${ROOT_DIR}/assets/icons/deckplayer.svg" "${APPDIR}/deckplayer.svg"

cat > "${APPDIR}/usr/bin/qt.conf" <<'EOF'
[Paths]
Prefix=..
Plugins=plugins
Qml2Imports=qml
EOF

if [[ -z "${QT_PLUGIN_DIR}" || -z "${QT_QML_DIR}" ]]; then
    cat >&2 <<EOF
Unable to locate the Qt 6 plugin and QML import directories automatically.

If your distro uses non-standard paths, set:
  QT_PLUGIN_DIR=/path/to/qt6/plugins
  QT_QML_DIR=/path/to/qt6/qml
EOF
    exit 1
fi

copy_if_exists "${QT_PLUGIN_DIR}/platforms" "${APPDIR}/usr/plugins"
copy_if_exists "${QT_PLUGIN_DIR}/imageformats" "${APPDIR}/usr/plugins"
copy_if_exists "${QT_PLUGIN_DIR}/platforminputcontexts" "${APPDIR}/usr/plugins"
copy_if_exists "${QT_PLUGIN_DIR}/wayland-decoration-client" "${APPDIR}/usr/plugins"
copy_if_exists "${QT_PLUGIN_DIR}/wayland-graphics-integration-client" "${APPDIR}/usr/plugins"
copy_if_exists "${QT_PLUGIN_DIR}/wayland-shell-integration" "${APPDIR}/usr/plugins"
copy_glob_if_exists "/usr/lib*/libQt6XcbQpa.so*" "${APPDIR}/usr/lib"
copy_glob_if_exists "/usr/lib*/*/libQt6XcbQpa.so*" "${APPDIR}/usr/lib"
copy_glob_if_exists "/usr/lib*/libxcb-cursor.so*" "${APPDIR}/usr/lib"
copy_glob_if_exists "/usr/lib*/*/libxcb-cursor.so*" "${APPDIR}/usr/lib"
copy_glob_if_exists "/usr/lib*/libxcb-icccm.so*" "${APPDIR}/usr/lib"
copy_glob_if_exists "/usr/lib*/*/libxcb-icccm.so*" "${APPDIR}/usr/lib"
copy_glob_if_exists "/usr/lib*/libxcb-image.so*" "${APPDIR}/usr/lib"
copy_glob_if_exists "/usr/lib*/*/libxcb-image.so*" "${APPDIR}/usr/lib"
copy_glob_if_exists "/usr/lib*/libxcb-keysyms.so*" "${APPDIR}/usr/lib"
copy_glob_if_exists "/usr/lib*/*/libxcb-keysyms.so*" "${APPDIR}/usr/lib"
copy_glob_if_exists "/usr/lib*/libxcb-render-util.so*" "${APPDIR}/usr/lib"
copy_glob_if_exists "/usr/lib*/*/libxcb-render-util.so*" "${APPDIR}/usr/lib"
copy_glob_if_exists "/usr/lib*/libxcb-xinput.so*" "${APPDIR}/usr/lib"
copy_glob_if_exists "/usr/lib*/*/libxcb-xinput.so*" "${APPDIR}/usr/lib"
copy_glob_if_exists "/usr/lib*/libxcb-xkb.so*" "${APPDIR}/usr/lib"
copy_glob_if_exists "/usr/lib*/*/libxcb-xkb.so*" "${APPDIR}/usr/lib"
copy_glob_if_exists "/usr/lib*/libxkbcommon-x11.so*" "${APPDIR}/usr/lib"
copy_glob_if_exists "/usr/lib*/*/libxkbcommon-x11.so*" "${APPDIR}/usr/lib"
copy_if_exists "${QT_QML_DIR}/QtCore" "${APPDIR}/usr/qml"
copy_if_exists "${QT_QML_DIR}/QtQml" "${APPDIR}/usr/qml"
copy_if_exists "${QT_QML_DIR}/QtQuick" "${APPDIR}/usr/qml"
if [[ -n "${MUJS_LIB_GLOB}" ]]; then
    copy_glob_if_exists "${MUJS_LIB_GLOB}" "${APPDIR}/usr/lib"
fi

copy_qt_library libQt6QmlWorkerScript
copy_qt_library libQt6QuickControls2
copy_qt_library libQt6QuickControls2Impl
copy_qt_library libQt6QuickTemplates2
copy_qt_library libQt6QmlMeta

APPIMAGE_EXTRACT_AND_RUN=1 NO_STRIP=1 LINUXDEPLOY_NO_STRIP=1 "${LINUXDEPLOY_BIN}" \
    --appdir "${APPDIR}" \
    --desktop-file "${APPDIR}/deckplayer.desktop" \
    --icon-file "${APPDIR}/deckplayer.svg"

mkdir -p "${APPDIR}/usr/optional-libs"
copy_glob_if_exists "${APPDIR}/usr/lib/libavformat.so*" "${APPDIR}/usr/optional-libs"
{
    declare -A bundled_sonames=()
    for library_glob in \
        libavutil.so.* \
        libswresample.so.* \
        libswscale.so.* \
        libavcodec.so.* \
        libxml2.so.* \
        libavformat.so.* \
        libavfilter.so.* \
        libavdevice.so.*; do
        for bundled_library in "${APPDIR}"/usr/lib/${library_glob} "${APPDIR}"/usr/optional-libs/${library_glob}; do
            [[ -e "${bundled_library}" ]] || continue
            soname="$(readelf -d "${bundled_library}" 2>/dev/null |
                sed -n 's/.*(SONAME).*Shared library: \[\(.*\)\].*/\1/p' |
                head -n 1)"
            soname="${soname:-$(basename "${bundled_library}")}"
            bundled_sonames["${soname}"]=1
        done
    done
    printf '%s\n' "${!bundled_sonames[@]}" | sort
} > "${APPDIR}/usr/optional-libs/ffmpeg-preload-libs.txt"

rm -f "${APPDIR}/AppRun"
cat > "${APPDIR}/AppRun" <<'EOF'
#!/usr/bin/env bash

set -euo pipefail

APPDIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
APP_LIB_DIR="${APPDIR}/usr/lib"
OPTIONAL_LIB_DIR="${APPDIR}/usr/optional-libs"
FFMPEG_PRELOAD_MANIFEST="${OPTIONAL_LIB_DIR}/ffmpeg-preload-libs.txt"

export LD_LIBRARY_PATH="${APP_LIB_DIR}${LD_LIBRARY_PATH:+:${LD_LIBRARY_PATH}}"

host_has_bundled_libavformat=0
while IFS= read -r library_name; do
    if [[ "${library_name}" == libavformat.so.* ]] &&
        ldconfig -p 2>/dev/null | grep -Fq "${library_name}"; then
        host_has_bundled_libavformat=1
        break
    fi
done < "${FFMPEG_PRELOAD_MANIFEST}"

if [[ "${STEAMDECKMEDIAPLAYER_USE_BUNDLED_FFMPEG:-0}" == "1" ]] ||
    [[ "${host_has_bundled_libavformat}" != "1" ]]; then
    bundled_ffmpeg_preload=""
    while IFS= read -r library_name; do
        bundled_lib="${APP_LIB_DIR}/${library_name}"
        if [[ ! -e "${bundled_lib}" && "${library_name}" == libavformat.so.* ]]; then
            bundled_lib="${OPTIONAL_LIB_DIR}/${library_name}"
        fi
        if [[ -e "${bundled_lib}" ]]; then
            bundled_ffmpeg_preload="${bundled_ffmpeg_preload}${bundled_ffmpeg_preload:+:}${bundled_lib}"
        fi
    done < "${FFMPEG_PRELOAD_MANIFEST}"
    export LD_LIBRARY_PATH="${OPTIONAL_LIB_DIR}:${LD_LIBRARY_PATH}"
    export LD_PRELOAD="${bundled_ffmpeg_preload}${LD_PRELOAD:+:${LD_PRELOAD}}"
fi

exec "${APPDIR}/usr/bin/deckplayer" "$@"
EOF
chmod +x "${APPDIR}/AppRun"

APPIMAGE_EXTRACT_AND_RUN=1 "${APPIMAGETOOL_BIN}" "${APPDIR}"

mv -f "${ROOT_DIR}"/Steam_Deck_Media_Player-*.AppImage "${OUTPUT_DIR}/" 2>/dev/null || true

echo "AppImage output:"
find "${OUTPUT_DIR}" -maxdepth 1 -name "*.AppImage" -print
