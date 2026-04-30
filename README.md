# Steam Deck Media Player

Steam Deck Media Player is a small local-first media browser and player aimed at Steam Deck Game Mode and KDE desktop use. The target workflow is simple: launch from Steam as a non-Steam game, browse `~/Videos` with controller-friendly inputs, and play files stored locally on the device or SD card.

## V1 goals

- Start in `~/Videos`
- Exclude hidden files and folders by default, with a toggle to reveal them
- Show folders first, then likely media files
- Support a `Resume / Start Over` prompt for partially watched files
- Keep the UI large, simple, and reliable with Steam Input mappings

## Stack

- `Qt 6 + QML` for the shell and UI
- `libmpv` for playback
- `CMake` for builds
- `AppImage` planned for distribution

## Current status

The repository currently includes:

- a Qt/QML application skeleton
- a folder-first browser model rooted from the user's filesystem
- persistence for last visited folder, hidden-file toggle, and saved resume positions
- embedded `libmpv` playback with pause, seek, subtitle cycling, and audio track cycling
- controller-friendly keyboard mappings that are easy to bind through Steam Input

## Build

You need Qt 6.5 or newer with `Core`, `Gui`, `Qml`, and `Quick`, plus the `libmpv` development package available through `pkg-config` as `mpv`.

```bash
cmake -S . -B build
cmake --build build
```

## Controls

- Browser: arrows or D-pad move, `Enter`/`Right` opens, `Escape`/`Left` goes back
- Browser: `W`/`S` also move, and `Q`/`E` or `Page Up`/`Page Down` jump faster through long folders
- Browser: `D` also opens and `A` also goes back for Steam Input layouts that prefer face-button letters
- Player: `Enter` or `P` pauses, `Left`/`Q` seeks backward, `Right`/`E` seeks forward
- Player: `X` cycles subtitles, `Y` cycles audio tracks, `Escape` exits playback
- Window: `F` or `F11` toggles fullscreen

## Steam Deck packaging

The Linux packaging assets live under [packaging/linux](/Users/justin/Documents/GitHub/steamdeckmediaplayer/packaging/linux/build-appimage.sh:1).

To build an AppImage on Linux:

```bash
./packaging/linux/build-appimage.sh
```

That script expects these tools on the Linux build machine:

- `cmake`
- `pkg-config`
- `linuxdeploy`
- `appimagetool`
- Qt 6 development packages
- `libmpv` development packages

The script writes the final AppImage to `dist/`.

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
- one back grip or extra button to `F` for fullscreen toggle

## Next implementation steps

1. Add richer OSD state for subtitle/audio track names and seek feedback
2. Add direct gamepad support alongside Steam Input keyboard mappings
3. Add AppImage packaging
4. Test and tune behavior on actual Steam Deck hardware
