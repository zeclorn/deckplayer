cmake_minimum_required(VERSION 3.21)

project(deckplayer VERSION 1.2.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(PkgConfig REQUIRED)
find_package(Qt6 6.4 REQUIRED COMPONENTS Core Gui Qml Quick OpenGL)
pkg_check_modules(LIBMPV REQUIRED IMPORTED_TARGET mpv)
pkg_check_modules(SDL2 REQUIRED IMPORTED_TARGET sdl2)

qt_standard_project_setup(REQUIRES 6.4)
if(QT_KNOWN_POLICY_QTP0001)
    qt_policy(SET QTP0001 NEW)
endif()
if(QT_KNOWN_POLICY_QTP0004)
    qt_policy(SET QTP0004 NEW)
endif()

qt_add_executable(deckplayer
    src/main.cpp
    src/AppState.cpp
    src/ControllerInput.cpp
    src/FileBrowserModel.cpp
    src/MpvVideoItem.cpp
)

set_target_properties(deckplayer PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

target_include_directories(deckplayer PRIVATE src)

qt_add_qml_module(deckplayer
    URI DeckPlayer
    VERSION 1.0
    QML_FILES
        qml/Main.qml
        qml/BrowserView.qml
        qml/PlayerView.qml
)

target_sources(deckplayer PRIVATE
    src/AppState.h
    src/ControllerInput.h
    src/FileBrowserModel.h
    src/MpvVideoItem.h
)

target_link_libraries(deckplayer
    PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Qml
        Qt6::Quick
        Qt6::OpenGL
        PkgConfig::LIBMPV
        PkgConfig::SDL2
)

install(TARGETS deckplayer
    BUNDLE DESTINATION .
    RUNTIME DESTINATION bin
)

install(FILES packaging/flatpak/io.github.zeclorn.DeckPlayer.desktop
    DESTINATION share/applications
)

install(FILES assets/icons/deckplayer.svg
    DESTINATION share/icons/hicolor/scalable/apps
)
