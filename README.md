# Steam Deck Media Player

Steam Deck Media Player is a small local-first media browser and player aimed at Steam Deck Game Mode and KDE desktop use. The target workflow is simple: launch from Steam as a non-Steam game, browse `~/Videos` with controller-friendly inputs, and play files stored locally on the device or SD card.

## V1 goals

- Start in `~/Videos`
- Exclude hidden files and folders by default, with a toggle to reveal them
- Show folders first, then likely media files
- Always start videos from the beginning
- Keep the UI large, simple, and reliable with Steam Input mappings

## Stack

- `Qt 6 + QML` for the shell and UI
- `libmpv` for playback
- `CMake` for builds
- `Flatpak` for Steam Deck distribution

## Current status — v1.1.0

- Folder-first file browser rooted from the user's filesystem
- Persistence for last visited folder and hidden-file toggle
- Embedded `libmpv` playback with pause, seek, subtitle cycling, and audio track cycling
- Playback OSD feedback for pause/play, seek, subtitle changes, and audio track changes
- Controller-friendly keyboard mappings that are easy to bind through Steam Input
- Direct SDL gamepad input for Steam Deck controls and standard controllers
- Volume control (LB/RB in player)
- Per-file playback resume (saves and restores position across sessions)
- Exit confirmation hold gesture (hold B for 2 seconds) to prevent accidental closes

## Build

You need Qt 6.5 or newer with `Core`, `Gui`, `Qml`, and `Quick`, plus the `libmpv` and SDL2 development packages available through `pkg-config` as `mpv` and `sdl2`.

```bash
cmake -S . -B build
cmake --build build
```

## Controls

- Browser: arrows or D-pad move, `Enter` opens, `Escape` goes back
- Browser: `W`/`S` also move, and `Q`/`E` or `Page Up`/`Page Down` jump faster through long folders
- Browser: `D` also opens and `A` also goes back for Steam Input layouts that prefer face-button letters
- Player: `Enter` or `P` pauses, `Left`/`Q` seeks backward, `Right`/`E` seeks forward
- Player: `X` cycles subtitles, `Y` cycles audio tracks, `Escape` exits playback
- Window: `F` or `F11` toggles fullscreen

Native gamepad controls are also supported:

- Browser: D-pad or left stick moves, `A` opens, `B` goes back, `LB`/`RB` jumps faster
- Player: `A` or `Start` pauses, D-pad/stick left or `LB` seeks backward, D-pad/stick right or `RB` seeks forward
- Player: `X` cycles subtitles, `Y` cycles audio tracks, `B` exits playback
- Window: `Select` exits the app

## Steam Deck packaging

The recommended Steam Deck package is Flatpak. The Flatpak manifest builds the
app, libmpv, and the media playback dependencies inside the KDE Flatpak runtime,
which avoids depending on the host distro's glibc and Qt library versions.

To build a local Flatpak bundle:

```bash
./packaging/flatpak/build-flatpak.sh
```

The script installs the KDE runtime, KDE SDK, and Flatpak Builder from Flathub
when they are missing. It writes the bundle to:

```text
dist/io.github.zeclorn.SteamDeckMediaPlayer.flatpak
```

Install and run the local bundle with:

```bash
flatpak install --user dist/io.github.zeclorn.SteamDeckMediaPlayer.flatpak
flatpak run io.github.zeclorn.SteamDeckMediaPlayer
```

The older AppImage packaging script remains under
[packaging/linux](./packaging/linux/build-appimage.sh:1), but it is no longer
the preferred Steam Deck distribution path.

## Recommended Steam Input mapping

For early Steam Deck testing as a non-Steam game, map:

- D-pad or left stick to arrow keys
- `A` button to `Enter`
- `B` button to `Escape`
- `X` button to `X`
- `Y` button to `Y`
- `LB` to `Q`
- `RB` to `E`
- `Start` to `P`
- `Select` or one back grip to quit the app
- one back grip or extra button to `F` for fullscreen toggle

## Release history

### v1.1.0 — 2026-05-31

- Volume control via LB/RB in the player
- Per-file playback resume (position saved and restored across sessions)
- Exit confirmation hold gesture (hold B for 2 seconds) to prevent accidental closes
- Exit confirmation dialog readability improvements
- Bug fixes for A/B button confirm and close flows

### v1.0.0 — 2026-05-01

First stable release. Flatpak packaging, controller-friendly browsing, direct Steam Deck gamepad support, local video playback, subtitle/audio track cycling, and fading playback overlays. Smoke-tested on Steam Deck Game Mode.
