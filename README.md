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
- `libmpv` planned for playback
- `CMake` for builds
- `AppImage` planned for distribution

## Current status

The repository currently includes:

- a Qt/QML application skeleton
- a folder-first browser model rooted from the user's filesystem
- persistence for last visited folder, hidden-file toggle, and saved resume positions
- a placeholder player screen that marks where `libmpv` integration will land next

## Build

You need Qt 6.5 or newer with `Core`, `Gui`, `Qml`, and `Quick`.

```bash
cmake -S . -B build
cmake --build build
```

## Next implementation steps

1. Replace the placeholder player view with embedded `libmpv`
2. Add real playback controls for seek, subtitle selection, and audio track switching
3. Add direct gamepad support alongside Steam Input keyboard mappings
4. Add AppImage packaging
