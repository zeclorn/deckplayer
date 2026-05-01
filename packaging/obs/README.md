# Open Build Service Notes

This repository can be built in OBS for two slightly different goals:

1. build a native package on an older distro target such as `openSUSE_Leap_15.6`
2. use that older target to produce the AppImage from a glibc baseline that is
   closer to SteamOS

The second goal is why this folder exists. Building the AppImage on a newer
rolling distro can bundle libraries that do not run on Steam Deck.

## Recommended target

Use an older target with a conservative glibc baseline, for example:

- `openSUSE_Leap_15.6`

That target should be treated as the compatibility floor for the AppImage.

## What the AppImage script expects

[`packaging/linux/build-appimage.sh`](./packaging/linux/build-appimage.sh:1)
now detects Qt plugin and QML import paths automatically, and it also accepts
these overrides for OBS or custom chroots:

- `QT_PLUGIN_DIR`
- `QT_QML_DIR`
- `LINUXDEPLOY_BIN`
- `APPIMAGETOOL_BIN`
- `BUILD_DIR`
- `APPDIR`
- `OUTPUT_DIR`

That means an OBS spec can point the script at `%{_libdir}/qt6/plugins` and
`%{_libdir}/qt6/qml` without patching the script per distro.

## Suggested OBS workflow

1. Create an OBS package for this repository.
2. Target `openSUSE_Leap_15.6` first.
3. Upload a source archive plus [`packaging/obs/steamdeckmediaplayer.spec`](./packaging/obs/steamdeckmediaplayer.spec:1).
4. Start by building the native RPM to verify the Qt 6 and `libmpv` dependency
   names on your project target.
5. After the native package builds cleanly, add `linuxdeploy` and
   `appimagetool` to the OBS project or package repository and switch the `%install`
   phase to invoke the AppImage script if you want a wrapped AppImage RPM.

[`packaging/obs/install-appimage.sh`](./packaging/obs/install-appimage.sh:1)
is a helper for that second phase. It runs the AppImage build with OBS-friendly
output directories and installs the generated AppImage into `DESTDIR`.

## Why start with the native RPM

OBS dependency names differ a bit by distro, and the AppImage tool packages are
often the part that needs local project tuning. The native package is the
fastest way to confirm:

- Qt 6 devel packages are present under the names your target expects
- `pkgconfig(mpv)` resolves correctly
- the app itself builds on the older glibc baseline

Once that works, the AppImage step is much more mechanical.
