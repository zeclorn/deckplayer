import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Window
import SteamDeckMediaPlayer

ApplicationWindow {
    id: window
    width: 1280
    height: 800
    visible: true
    title: "Steam Deck Media Player"
    color: "#101922"
    visibility: Qt.platform.os === "linux" ? Window.FullScreen : Window.Windowed

    function toggleFullscreen() {
        if (visibility === Window.FullScreen) {
            visibility = Window.Windowed
        } else {
            visibility = Window.FullScreen
        }
    }

    Keys.onPressed: event => {
        if (event.key === Qt.Key_F11 || event.key === Qt.Key_F
                || (event.key === Qt.Key_Return && (event.modifiers & Qt.AltModifier))) {
            toggleFullscreen()
            event.accepted = true
        }
    }

    StackLayout {
        anchors.fill: parent
        currentIndex: AppState.playerVisible ? 1 : 0

        BrowserView {
            Layout.fillWidth: true
            Layout.fillHeight: true
        }

        PlayerView {
            Layout.fillWidth: true
            Layout.fillHeight: true
        }
    }

    ResumeDialog {
        anchors.centerIn: parent
        visible: AppState.resumePromptVisible
        mediaTitle: AppState.pendingMediaTitle
        resumePositionLabel: AppState.formatDuration(AppState.pendingResumePositionMs)
        onResumeRequested: AppState.chooseResume()
        onStartOverRequested: AppState.chooseStartOver()
        onCancelled: AppState.goBack()
    }
}
