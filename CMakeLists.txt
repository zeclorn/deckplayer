cmake_minimum_required(VERSION 3.21)

project(steamdeckmediaplayer VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(PkgConfig REQUIRED)
find_package(Qt6 6.5 REQUIRED COMPONENTS Core Gui Qml Quick)
pkg_check_modules(LIBMPV REQUIRED IMPORTED_TARGET mpv)

qt_standard_project_setup(REQUIRES 6.5)

qt_add_executable(steamdeckmediaplayer
    src/main.cpp
    src/AppState.cpp
    src/FileBrowserModel.cpp
    src/MpvVideoItem.cpp
)

set_target_properties(steamdeckmediaplayer PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

target_include_directories(steamdeckmediaplayer PRIVATE src)

qt_add_qml_module(steamdeckmediaplayer
    URI SteamDeckMediaPlayer
    VERSION 1.0
    QML_FILES
        qml/Main.qml
        qml/BrowserView.qml
        qml/PlayerView.qml
        qml/ResumeDialog.qml
)

target_sources(steamdeckmediaplayer PRIVATE
    src/AppState.h
    src/FileBrowserModel.h
    src/MpvVideoItem.h
)

target_link_libraries(steamdeckmediaplayer
    PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Qml
        Qt6::Quick
        PkgConfig::LIBMPV
)

install(TARGETS steamdeckmediaplayer
    BUNDLE DESTINATION .
    RUNTIME DESTINATION bin
)
