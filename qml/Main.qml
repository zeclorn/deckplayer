import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Window
import DeckPlayer

ApplicationWindow {
    id: window
    width: 1280
    height: 800
    visible: true
    title: "DeckPlayer"
    color: "#101922"
    visibility: Qt.platform.os === "linux" ? Window.FullScreen : Window.Windowed

    function toggleFullscreen() {
        if (visibility === Window.FullScreen) {
            visibility = Window.Windowed
        } else {
            visibility = Window.FullScreen
        }
    }

    Shortcut {
        sequences: ["F", "F11", "Alt+Return"]
        onActivated: window.toggleFullscreen()
    }

    Connections {
        target: ControllerInput

        function onActionPressed(action) {
            if (action === "fullscreen") {
                window.toggleFullscreen()
            }
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
}
