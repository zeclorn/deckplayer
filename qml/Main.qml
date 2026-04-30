import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ApplicationWindow {
    id: window
    width: 1280
    height: 800
    visible: true
    title: "Steam Deck Media Player"
    color: "#101922"

    StackLayout {
        anchors.fill: parent
        currentIndex: appState.playerVisible ? 1 : 0

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
        visible: appState.resumePromptVisible
        mediaTitle: appState.pendingMediaTitle
        resumePositionLabel: appState.formatDuration(appState.pendingResumePositionMs)
        onResumeRequested: appState.chooseResume()
        onStartOverRequested: appState.chooseStartOver()
        onCancelled: appState.goBack()
    }
}
