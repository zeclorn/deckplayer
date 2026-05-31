# CLAUDE.md — Steam Deck Media Player

This file documents the codebase structure, build workflows, and conventions for AI assistants working in this repository.

## Project Overview

Steam Deck Media Player is a lightweight, controller-friendly media browser and player written in **C++20 with Qt 6 / QML**. It targets Steam Deck Game Mode and KDE desktop environments, using **libmpv** for video playback and **SDL2** for gamepad input.

- **Version:** 1.0.0
- **License:** MIT
- **Primary distribution:** Flatpak

---

## Repository Structure

```
steamdeckmediaplayer/
├── CMakeLists.txt                  # Build configuration (CMake 3.21+, Qt 6.4+, C++20)
├── README.md                       # User-facing docs, control mappings, release checklist
├── LICENSE
├── .gitignore
│
├── src/                            # C++ backend
│   ├── main.cpp                    # Qt app entry point; registers C++ types with QML
│   ├── AppState.h/cpp              # Global app state singleton (QSettings persistence)
│   ├── FileBrowserModel.h/cpp      # QAbstractListModel for directory traversal
│   ├── ControllerInput.h/cpp       # SDL2 gamepad polling (60 Hz, direction repeat)
│   └── MpvVideoItem.h/cpp          # QQuickFramebufferObject wrapping libmpv + OpenGL
│
├── qml/                            # QML frontend
│   ├── Main.qml                    # Root ApplicationWindow; view switching
│   ├── BrowserView.qml             # File browser UI
│   └── PlayerView.qml              # Video player UI with OSD overlays
│
├── assets/icons/
│   └── steamdeckmediaplayer.svg
│
└── packaging/
    ├── flatpak/                    # Primary packaging target
    │   ├── io.github.zeclorn.SteamDeckMediaPlayer.yml   # Flatpak manifest
    │   ├── io.github.zeclorn.SteamDeckMediaPlayer.desktop
    │   ├── io.github.zeclorn.SteamDeckMediaPlayer.metainfo.xml
    │   └── build-flatpak.sh        # Builds bundle to dist/
    ├── linux/                      # AppImage (legacy, not recommended for Steam Deck)
    │   ├── build-appimage.sh
    │   └── steamdeckmediaplayer.desktop
    └── obs/                        # OBS (Open Build Service) packaging
        ├── README.md
        ├── steamdeckmediaplayer.spec
        └── install-appimage.sh
```

---

## Architecture

### C++ / QML Split

The app follows a clean backend/frontend separation:

- **C++ (`src/`)** — models, input handling, video rendering, persistence
- **QML (`qml/`)** — all UI layout, animations, and OSD logic

C++ objects are registered as QML singletons in `main.cpp`:

```cpp
// AppState and ControllerInput are singletons accessible from any QML file
qmlRegisterSingletonInstance("SteamDeckMediaPlayer", 1, 0, "AppState", &appState);
```

### Key Patterns

| Pattern | Where used |
|---------|-----------|
| `QAbstractListModel` (Model-View) | `FileBrowserModel` → `BrowserView.qml` |
| `QQuickFramebufferObject` | `MpvVideoItem` — OpenGL video frames via libmpv |
| Qt signals/slots | All C++↔QML communication |
| `QSettings` persistence | Last browser path, hidden-file toggle, per-file resume positions |
| SDL polling timer | `ControllerInput` polls every 16 ms (60 Hz); direction key repeat: 500 ms initial / 180 ms interval |

### AppState

Centralized state owned by `AppState` (singleton):
- `playerVisible` — whether the player or browser is shown
- `currentMedia` — path of the file being played
- Browser scroll position and hidden-file toggle

### MpvVideoItem

`MpvVideoItem` is the most complex class (~658 lines). It:
- Wraps libmpv with OpenGL rendering via `QQuickFramebufferObject`
- Exposes `play()`, `pause()`, `seek()`, `cycleSubtitles()`, `cycleAudio()`, `setVolume()` as Q_INVOKABLE methods
- Saves and restores per-file playback positions (base64-encoded QSettings key)
- Sets `LC_NUMERIC` to `"C"` to ensure consistent number parsing in libmpv property strings

---

## Building

### Native Build (development)

```bash
# Configure
cmake -S . -B build

# Build
cmake --build build

# Run
./build/steamdeckmediaplayer
```

**Required dependencies:**
- CMake ≥ 3.21
- Qt 6.4+ (`Qt6::Core`, `Qt6::Gui`, `Qt6::Qml`, `Qt6::Quick`)
- libmpv (detected via `pkg-config`)
- SDL2 (detected via `pkg-config`)
- C++20-capable compiler (GCC/Clang)

### Flatpak Build (primary / Steam Deck)

```bash
cd packaging/flatpak
./build-flatpak.sh
# Output: dist/io.github.zeclorn.SteamDeckMediaPlayer.flatpak
```

The script auto-installs `org.kde.Platform//6.10` and `org.kde.Sdk//6.10` if missing. The Flatpak manifest builds libass, libplacebo, and mpv from source before building the app.

### AppImage Build (legacy)

```bash
cd packaging/linux
./build-appimage.sh
```

Not recommended for Steam Deck. Prefer Flatpak.

---

## Testing

There is **no automated test suite**. Testing is manual.

**Manual smoke test checklist** (from README.md):
1. Install and launch via Flatpak
2. Browse `~/Videos` and an SD card path
3. Play a video, seek forward/back, check OSD
4. Cycle subtitle and audio tracks
5. Verify exit confirmation (hold B for 2 seconds)
6. Verify playback resume (exit mid-video, reopen same file)
7. Test D-pad, analog stick navigation, and all button mappings

When making changes, manually verify the affected feature using the above workflow.

---

## Controller Input Conventions

| Button | Action |
|--------|--------|
| D-pad / Left stick | Navigate file list |
| A | Confirm / play selected file |
| B (tap) | Go up one directory / return to browser |
| B (hold 2 s) | Exit application |
| X | Toggle hidden files |
| Y | Toggle OSD overlay |
| Left trigger | Cycle subtitle tracks |
| Right trigger | Cycle audio tracks |
| Left bumper | Volume down |
| Right bumper | Volume up |
| Start | Seek forward 10 s |
| Select | Seek back 10 s |

---

## Media File Support

Recognized extensions (defined in `FileBrowserModel.cpp`):

```
mp4, m4v, mkv, avi, mov, webm, mpeg, mpg, ts, m2ts, wmv, flv
```

Directory listing: hidden files filtered by default; folders listed before media files.

---

## Code Conventions

- **Standard:** C++20 throughout (enforced in `CMakeLists.txt`)
- **No linter or formatter is configured.** Match the surrounding code style when editing.
- **No automated formatting tool** (clang-format, etc.) — format manually and consistently.
- Qt naming conventions apply: `camelCase` for methods/variables, `PascalCase` for classes.
- QML properties use `camelCase`; component IDs use `camelCase` (e.g., `id: playerView`).
- Keep C++ logic in `src/`; keep all visual/layout decisions in `qml/`.
- Do not add UI logic to C++ and do not add business logic to QML.

---

## Packaging & Distribution

| Format | Status | Notes |
|--------|--------|-------|
| Flatpak | Primary | KDE Platform 6.10; targets Steam Deck |
| AppImage | Legacy | `packaging/linux/`; less maintained |
| OBS | Available | RPM spec in `packaging/obs/` |

App ID: `io.github.zeclorn.SteamDeckMediaPlayer`

---

## CI/CD

No CI/CD pipelines exist. There is no `.github/workflows/` directory. All builds and releases are done manually via the packaging scripts above.

---

## Git Workflow

- Default development branch: `main`
- Feature branches follow the pattern `claude/<description>` for AI-assisted work
- Commit messages are imperative, short, and descriptive (e.g., `Fix B button exit flow`)
- No enforced commit hooks

---

## Common Tasks for AI Assistants

### Adding a new media file extension

Edit `FileBrowserModel.cpp` — find the `mediaExtensions` set and add the new extension in lowercase.

### Changing a controller button mapping

Edit `ControllerInput.cpp` — find the `SDL_GameControllerButton` or `SDL_GameControllerAxis` switch statement and update the emitted signal. Update `qml/PlayerView.qml` and/or `qml/BrowserView.qml` to handle the signal.

### Adding a new mpv property or command

Add a `Q_INVOKABLE` method in `MpvVideoItem.h`, implement it in `MpvVideoItem.cpp` using `mpv_command_async` or `mpv_set_property_async`, then call the method from QML.

### Adding persistent settings

Use `QSettings` in `AppState.cpp`. Define a `Q_PROPERTY` with `READ`/`WRITE`/`NOTIFY`, load in the constructor, and save on change.

### Modifying the OSD

All OSD elements (progress bar, time display, subtitle/audio indicators) are in `qml/PlayerView.qml`. The fade-in/out is controlled by `PropertyAnimation` tied to `osdVisible`.
