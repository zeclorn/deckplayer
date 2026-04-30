import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import SteamDeckMediaPlayer

FocusScope {
    id: root
    focus: true

    function closePlayerAndPersist() {
        if (!player.loaded && player.positionMs <= 0) {
            AppState.closePlayer()
            return
        }

        if (player.durationMs > 0 && player.positionMs >= Math.max(0, player.durationMs - 60000)) {
            AppState.clearResume(AppState.currentMediaPath)
        } else if (player.positionMs > 0) {
            AppState.savePlaybackPosition(player.positionMs)
        }

        player.stop()
        AppState.closePlayer()
    }

    Rectangle {
        anchors.fill: parent
        color: "#05080b"
    }

    Keys.onPressed: event => {
        if (event.key === Qt.Key_Escape || event.key === Qt.Key_Backspace) {
            closePlayerAndPersist()
            event.accepted = true
        } else if (event.key === Qt.Key_Space || event.key === Qt.Key_Return
                || event.key === Qt.Key_Enter || event.key === Qt.Key_P) {
            player.togglePause()
            event.accepted = true
        } else if (event.key === Qt.Key_Left || event.key === Qt.Key_Q) {
            player.seekBackward()
            event.accepted = true
        } else if (event.key === Qt.Key_Right || event.key === Qt.Key_E) {
            player.seekForward()
            event.accepted = true
        } else if (event.key === Qt.Key_X) {
            player.cycleSubtitles()
            event.accepted = true
        } else if (event.key === Qt.Key_Y) {
            player.cycleAudioTracks()
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
                    AppState.clearResume(AppState.currentMediaPath)
                    AppState.closePlayer()
                }

                onPlaybackStopped: {
                    if (AppState.playerVisible) {
                        AppState.closePlayer()
                    }
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
                        onClicked: player.togglePause()
                    }

                    Button {
                        text: "-10s"
                        onClicked: player.seekBackward()
                    }

                    Button {
                        text: "+10s"
                        onClicked: player.seekForward()
                    }

                    Button {
                        text: "Subtitles"
                        onClicked: player.cycleSubtitles()
                    }

                    Button {
                        text: "Audio"
                        onClicked: player.cycleAudioTracks()
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
            text: "Controls: A/Enter/P pause, Left/Q and Right/E seek, X subtitles, Y audio, B/Escape back, F fullscreen"
            color: "#93a7bb"
            font.pixelSize: 18
        }
    }
}
