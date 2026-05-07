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
    property double confirmingExitAtMs: 0
    property string osdText: ""
    property bool osdVisible: false
    property bool controlsVisible: true
    property bool confirmingExit: false

    Connections {
        target: AppState

        function onCurrentMediaChanged() {
            root.openedAtMs = Date.now()
            root.lastPrimaryActionMs = root.openedAtMs
            root.confirmingExit = false
            root.forceActiveFocus()
        }

        function onPlayerVisibleChanged() {
            if (AppState.playerVisible && root.visible) {
                root.openedAtMs = Date.now()
                root.lastPrimaryActionMs = root.openedAtMs
                root.confirmingExit = false
                root.forceActiveFocus()
                root.showControls()
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
        showControls()
        osdText = text
        osdVisible = true
        osdTimer.restart()
    }

    function togglePauseWithOsd() {
        showControls()
        player.togglePause()
    }

    function seekBackwardWithOsd() {
        showControls()
        player.seekBackward()
        showOsd("-10s")
    }

    function seekForwardWithOsd() {
        showControls()
        player.seekForward()
        showOsd("+10s")
    }

    function volumeUpWithOsd() {
        player.volumeUp()
        showOsd("Volume: " + player.volume + "%")
    }

    function volumeDownWithOsd() {
        player.volumeDown()
        showOsd("Volume: " + player.volume + "%")
    }

    function cycleSubtitlesWithOsd() {
        showControls()
        trackActionAtMs = Date.now()
        player.cycleSubtitles()
        trackFeedbackFallbackTimer.feedbackText = "Subtitles"
        trackFeedbackFallbackTimer.restart()
    }

    function cycleAudioWithOsd() {
        showControls()
        trackActionAtMs = Date.now()
        player.cycleAudioTracks()
        trackFeedbackFallbackTimer.feedbackText = "Audio"
        trackFeedbackFallbackTimer.restart()
    }

    function showControls() {
        controlsVisible = true
        controlsFadeTimer.restart()
    }

    function savePositionAndClose() {
        if (player.loaded && player.positionMs > 5000 && player.durationMs > 0) {
            const nearEnd = player.positionMs >= player.durationMs * 0.95
            if (nearEnd) {
                AppState.clearPlaybackPosition(AppState.currentMediaPath)
            } else {
                AppState.savePlaybackPosition(AppState.currentMediaPath, player.positionMs)
            }
        }
        confirmingExit = false
        exitConfirmTimer.stop()
        AppState.closePlayer()
        player.stop()
    }

    function requestClose(force) {
        if (!force && Date.now() - trackActionAtMs < 450) {
            return
        }

        if (!force && player.loaded && !confirmingExit) {
            confirmingExit = true
            confirmingExitAtMs = Date.now()
            exitConfirmTimer.restart()
            showControls()
            return
        }

        savePositionAndClose()
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
        id: controlsFadeTimer
        interval: 2400
        onTriggered: root.controlsVisible = false
    }

    Timer {
        id: exitConfirmTimer
        interval: 5000
        onTriggered: root.confirmingExit = false
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
            root.showControls()

            if (root.confirmingExit) {
                if (action === "accept" || action === "playPause") {
                    root.savePositionAndClose()
                } else if (action === "cancel" && Date.now() - root.confirmingExitAtMs >= 400) {
                    root.confirmingExit = false
                    exitConfirmTimer.stop()
                }
                return
            }

            if (action === "cancel") {
                root.requestClose()
            } else if (action === "accept" || action === "playPause") {
                if (root.shouldAcceptPrimaryAction()) {
                    root.togglePauseWithOsd()
                }
            } else if (action === "left" || action === "leftShoulder") {
                root.seekBackwardWithOsd()
            } else if (action === "right" || action === "rightShoulder") {
                root.seekForwardWithOsd()
            } else if (action === "up") {
                root.volumeUpWithOsd()
            } else if (action === "down") {
                root.volumeDownWithOsd()
            } else if (action === "x") {
                root.cycleSubtitlesWithOsd()
            } else if (action === "y") {
                root.cycleAudioWithOsd()
            } else if (action === "quit") {
                root.savePositionAndClose()
                Qt.quit()
            }
        }
    }

    Keys.onPressed: event => {
        if (!AppState.playerVisible) {
            return
        }

        root.showControls()

        if (event.key === Qt.Key_Escape || event.key === Qt.Key_Backspace) {
            if (root.confirmingExit) {
                root.confirmingExit = false
                exitConfirmTimer.stop()
            } else {
                root.requestClose()
            }
            event.accepted = true
        } else if (event.key === Qt.Key_Space || event.key === Qt.Key_Return
                || event.key === Qt.Key_Enter || event.key === Qt.Key_P) {
            if (root.confirmingExit) {
                root.savePositionAndClose()
            } else if (root.shouldAcceptPrimaryAction()) {
                root.togglePauseWithOsd()
            }
            event.accepted = true
        } else if (event.key === Qt.Key_Left || event.key === Qt.Key_Q) {
            root.seekBackwardWithOsd()
            event.accepted = true
        } else if (event.key === Qt.Key_Right || event.key === Qt.Key_E) {
            root.seekForwardWithOsd()
            event.accepted = true
        } else if (event.key === Qt.Key_Up) {
            root.volumeUpWithOsd()
            event.accepted = true
        } else if (event.key === Qt.Key_Down) {
            root.volumeDownWithOsd()
            event.accepted = true
        } else if (event.key === Qt.Key_X) {
            root.cycleSubtitlesWithOsd()
            event.accepted = true
        } else if (event.key === Qt.Key_Y) {
            root.cycleAudioWithOsd()
            event.accepted = true
        }
    }

    HoverHandler {
        onHoveredChanged: if (hovered) root.showControls()
    }

    TapHandler {
        onTapped: root.showControls()
    }

    MpvVideoItem {
        id: player
        anchors.fill: parent
        mediaPath: AppState.currentMediaPath
        startPositionMs: AppState.playbackStartPositionMs
        requestId: AppState.playbackRequestId

        onPlaybackFinished: {
            AppState.clearPlaybackPosition(AppState.currentMediaPath)
            AppState.closePlayer()
        }

        onPlaybackStopped: {
            if (AppState.playerVisible) {
                AppState.closePlayer()
            }
        }

        onLoadedChanged: {
            if (loaded && AppState.playbackStartPositionMs > 0) {
                root.showOsd("Resuming from " + AppState.formatDuration(AppState.playbackStartPositionMs))
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

    Item {
        id: overlayChrome
        anchors.fill: parent
        opacity: root.controlsVisible ? 1 : 0
        visible: opacity > 0

        Behavior on opacity {
            NumberAnimation { duration: 220 }
        }

        Rectangle {
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            height: 100
            color: "#05080bcc"

            RowLayout {
                anchors.fill: parent
                anchors.leftMargin: 40
                anchors.rightMargin: 40
                spacing: 18

                Label {
                    text: AppState.currentMediaTitle
                    color: "#f4f7fb"
                    font.pixelSize: 30
                    font.weight: Font.DemiBold
                    elide: Text.ElideRight
                    Layout.fillWidth: true
                }

                Button {
                    text: "Back"
                    onClicked: {
                        root.showControls()
                        root.requestClose()
                    }
                }
            }
        }

        Rectangle {
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            height: 118
            color: "#05080bcc"

            RowLayout {
                anchors.fill: parent
                anchors.leftMargin: 40
                anchors.rightMargin: 40
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
                    text: "Vol -"
                    onClicked: root.volumeDownWithOsd()
                }

                Button {
                    text: "Vol +"
                    onClicked: root.volumeUpWithOsd()
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

    // Exit confirmation overlay
    Rectangle {
        anchors.centerIn: parent
        width: 460
        height: 150
        radius: 20
        color: "#05080bee"
        border.width: 1
        border.color: "#385168"
        visible: root.confirmingExit

        Column {
            anchors.centerIn: parent
            spacing: 18

            Label {
                anchors.horizontalCenter: parent.horizontalCenter
                text: "Exit playback?"
                color: "#f4f7fb"
                font.pixelSize: 28
                font.weight: Font.DemiBold
            }

            Label {
                anchors.horizontalCenter: parent.horizontalCenter
                text: "A  Confirm        B  Cancel"
                color: "#a0b4c8"
                font.pixelSize: 19
            }
        }
    }

    // OSD message bubble
    Rectangle {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 138
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
}
