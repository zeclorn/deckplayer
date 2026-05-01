#!/usr/bin/env bash

set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"
MANIFEST="${MANIFEST:-${ROOT_DIR}/packaging/flatpak/io.github.zeclorn.SteamDeckMediaPlayer.yml}"
BUILD_DIR="${BUILD_DIR:-${ROOT_DIR}/build-flatpak}"
REPO_DIR="${REPO_DIR:-${ROOT_DIR}/dist/flatpak-repo}"
BUNDLE_PATH="${BUNDLE_PATH:-${ROOT_DIR}/dist/io.github.zeclorn.SteamDeckMediaPlayer.flatpak}"
APP_ID="io.github.zeclorn.SteamDeckMediaPlayer"
RUNTIME_BRANCH="${RUNTIME_BRANCH:-6.9}"

if command -v flatpak-builder >/dev/null 2>&1; then
    BUILDER=(flatpak-builder)
else
    if ! flatpak info org.flatpak.Builder >/dev/null 2>&1; then
        flatpak install -y flathub org.flatpak.Builder
    fi
    BUILDER=(flatpak run org.flatpak.Builder)
fi

flatpak install -y flathub "org.kde.Platform//${RUNTIME_BRANCH}" "org.kde.Sdk//${RUNTIME_BRANCH}"

mkdir -p "$(dirname "${BUNDLE_PATH}")" "${REPO_DIR}"

"${BUILDER[@]}" \
    --force-clean \
    --repo="${REPO_DIR}" \
    "${BUILD_DIR}" \
    "${MANIFEST}"

flatpak build-bundle "${REPO_DIR}" "${BUNDLE_PATH}" "${APP_ID}"

echo "Flatpak bundle output:"
printf '%s\n' "${BUNDLE_PATH}"
