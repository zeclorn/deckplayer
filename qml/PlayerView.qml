import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import SteamDeckMediaPlayer

FocusScope {
    id: root
    focus: true
    property double lastPrimaryActionMs: 0
    property double openedAtMs: 0
    property double trackActionAtMs: 0
    property string osdText: ""
    property bool osdVisible: false

    Connections {
        target: AppState

        function onCurrentMediaChanged() {
            root.openedAtMs = Date.now()
            root.lastPrimaryActionMs = root.openedAtMs
            root.forceActiveFocus()
        }

        function onPlayerVisibleChanged() {
            if (AppState.playerVisible && root.visible) {
                root.openedAtMs = Date.now()
                root.lastPrimaryActionMs = root.openedAtMs
                root.forceActiveFocus()
            }
        }
    }

    function isInLaunchGuardWindow() {
        return Date.now() - openedAtMs < 300
    }

    function shouldAcceptPrimaryAction() {
        if (isInLaunchGuardWindow()) {
            return false
        }

        const now = Date.now()
        if (now - trackActionAtMs < 450) {
            return false
        }
        if (now - lastPrimaryActionMs < 350) {
            return false
        }
        lastPrimaryActionMs = now
        return true
    }

    function showOsd(text) {
        osdText = text
        osdVisible = true
        osdTimer.restart()
    }

    function togglePauseWithOsd() {
        player.togglePause()
    }

    function seekBackwardWithOsd() {
        player.seekBackward()
        showOsd("-10s")
    }

    function seekForwardWithOsd() {
        player.seekForward()
        showOsd("+10s")
    }

    function cycleSubtitlesWithOsd() {
        trackActionAtMs = Date.now()
        player.cycleSubtitles()
        trackFeedbackFallbackTimer.feedbackText = "Subtitles"
        trackFeedbackFallbackTimer.restart()
    }

    function cycleAudioWithOsd() {
        trackActionAtMs = Date.now()
        player.cycleAudioTracks()
        trackFeedbackFallbackTimer.feedbackText = "Audio"
        trackFeedbackFallbackTimer.restart()
    }

    function closePlayerAndPersist(force) {
        if (!force && isInLaunchGuardWindow()) {
            return
        }

        if (!force && Date.now() - trackActionAtMs < 450) {
            return
        }

        if (!force && !shouldAcceptPrimaryAction()) {
            return
        }

        player.stop()
        AppState.closePlayer()
    }

    Rectangle {
        anchors.fill: parent
        color: "#05080b"
    }

    Timer {
        id: osdTimer
        interval: 1400
        onTriggered: root.osdVisible = false
    }

    Timer {
        id: trackFeedbackFallbackTimer
        property string feedbackText: ""
        interval: 160
        onTriggered: root.showOsd(feedbackText)
    }

    Connections {
        target: ControllerInput
        enabled: root.visible && AppState.playerVisible

        function onActionPressed(action) {
            if (action === "cancel") {
                root.closePlayerAndPersist()
            } else if (action === "accept" || action === "playPause") {
                if (root.shouldAcceptPrimaryAction()) {
                    root.togglePauseWithOsd()
                }
            } else if (action === "left" || action === "leftShoulder") {
                root.seekBackwardWithOsd()
            } else if (action === "right" || action === "rightShoulder") {
                root.seekForwardWithOsd()
            } else if (action === "x") {
                root.cycleSubtitlesWithOsd()
            } else if (action === "y") {
                root.cycleAudioWithOsd()
            } else if (action === "quit") {
                root.closePlayerAndPersist(true)
                Qt.quit()
            }
        }
    }

    Keys.onPressed: event => {
        if (!AppState.playerVisible) {
            return
        }

        if (event.key === Qt.Key_Escape || event.key === Qt.Key_Backspace) {
            closePlayerAndPersist()
            event.accepted = true
        } else if (event.key === Qt.Key_Space || event.key === Qt.Key_Return
                || event.key === Qt.Key_Enter || event.key === Qt.Key_P) {
            if (root.shouldAcceptPrimaryAction()) {
                root.togglePauseWithOsd()
            }
            event.accepted = true
        } else if (event.key === Qt.Key_Left || event.key === Qt.Key_Q) {
            root.seekBackwardWithOsd()
            event.accepted = true
        } else if (event.key === Qt.Key_Right || event.key === Qt.Key_E) {
            root.seekForwardWithOsd()
            event.accepted = true
        } else if (event.key === Qt.Key_X) {
            root.cycleSubtitlesWithOsd()
            event.accepted = true
        } else if (event.key === Qt.Key_Y) {
            root.cycleAudioWithOsd()
            event.accepted = true
        }
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 40
        spacing: 24

        RowLayout {
            Layout.fillWidth: true

            Label {
                text: AppState.currentMediaTitle
                color: "#f4f7fb"
                font.pixelSize: 32
                font.weight: Font.DemiBold
                elide: Text.ElideRight
                Layout.fillWidth: true
            }

            Button {
                text: "Back"
                onClicked: root.closePlayerAndPersist()
            }
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            radius: 24
            color: "#101922"
            border.width: 1
            border.color: "#263749"
            clip: true

            MpvVideoItem {
                id: player
                anchors.fill: parent
                mediaPath: AppState.currentMediaPath
                startPositionMs: AppState.playbackStartPositionMs
                requestId: AppState.playbackRequestId

                onPlaybackFinished: {
                    AppState.closePlayer()
                }

                onPlaybackStopped: {
                    if (AppState.playerVisible) {
                        AppState.closePlayer()
                    }
                }

                onPausedChanged: {
                    if (AppState.playerVisible && !root.isInLaunchGuardWindow()) {
                        root.showOsd(paused ? "Paused" : "Playing")
                    }
                }

                onSubtitleTrackLabelChanged: {
                    if (AppState.playerVisible && loaded) {
                        trackFeedbackFallbackTimer.stop()
                        root.showOsd(subtitleTrackLabel)
                    }
                }

                onAudioTrackLabelChanged: {
                    if (AppState.playerVisible && loaded) {
                        trackFeedbackFallbackTimer.stop()
                        root.showOsd(audioTrackLabel)
                    }
                }
            }

            Rectangle {
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 126
                width: Math.min(parent.width - 80, Math.max(260, osdLabel.implicitWidth + 44))
                height: 58
                radius: 16
                color: "#05080bdd"
                border.width: 1
                border.color: "#385168"
                opacity: root.osdVisible ? 1 : 0
                visible: opacity > 0

                Behavior on opacity {
                    NumberAnimation { duration: 140 }
                }

                Label {
                    id: osdLabel
                    anchors.centerIn: parent
                    width: parent.width - 28
                    text: root.osdText
                    color: "#f4f7fb"
                    font.pixelSize: 24
                    font.weight: Font.DemiBold
                    horizontalAlignment: Text.AlignHCenter
                    elide: Text.ElideRight
                }
            }

            Rectangle {
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                anchors.margins: 18
                implicitHeight: 94
                radius: 18
                color: "#111923dd"
                border.width: 1
                border.color: "#2a3d4f"

                RowLayout {
                    anchors.fill: parent
                    anchors.leftMargin: 18
                    anchors.rightMargin: 18
                    spacing: 14

                    Button {
                        text: player.paused ? "Play" : "Pause"
                        onClicked: root.togglePauseWithOsd()
                    }

                    Button {
                        text: "-10s"
                        onClicked: root.seekBackwardWithOsd()
                    }

                    Button {
                        text: "+10s"
                        onClicked: root.seekForwardWithOsd()
                    }

                    Button {
                        text: "Subtitles"
                        onClicked: root.cycleSubtitlesWithOsd()
                    }

                    Button {
                        text: "Audio"
                        onClicked: root.cycleAudioWithOsd()
                    }

                    Item {
                        Layout.fillWidth: true
                    }

                    Label {
                        text: AppState.formatDuration(player.positionMs) + " / " + AppState.formatDuration(player.durationMs)
                        color: "#f4f7fb"
                        font.pixelSize: 20
                    }
                }
            }
        }

        Label {
            text: "Controls: A/Start/Enter/P pause, Left/LB/Q and Right/RB/E seek, X subtitles, Y audio, B/Escape back, Select exits"
            color: "#93a7bb"
            font.pixelSize: 18
        }
    }
}
