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

## Build

You need Qt 6.5 or newer with `Core`, `Gui`, `Qml`, and `Quick`, plus the `libmpv` development package available through `pkg-config` as `mpv`.

```bash
cmake -S . -B build
cmake --build build
```

## Next implementation steps

1. Add richer OSD state for subtitle/audio track names and seek feedback
2. Add direct gamepad support alongside Steam Input keyboard mappings
3. Add AppImage packaging
4. Test and tune behavior on actual Steam Deck hardware
