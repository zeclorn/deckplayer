import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

FocusScope {
    id: root
    focus: true

    Rectangle {
        anchors.fill: parent
        color: "#05080b"
    }

    Keys.onPressed: event => {
        if (event.key === Qt.Key_Escape || event.key === Qt.Key_Backspace) {
            appState.closePlayer()
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
                text: appState.currentMediaTitle
                color: "#f4f7fb"
                font.pixelSize: 32
                font.weight: Font.DemiBold
                elide: Text.ElideRight
                Layout.fillWidth: true
            }

            Button {
                text: "Back"
                onClicked: appState.closePlayer()
            }
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            radius: 24
            color: "#101922"
            border.width: 1
            border.color: "#263749"

            ColumnLayout {
                anchors.centerIn: parent
                width: parent.width - 80
                spacing: 20

                Label {
                    text: "Playback integration placeholder"
                    color: "#f4f7fb"
                    font.pixelSize: 34
                    horizontalAlignment: Text.AlignHCenter
                    Layout.fillWidth: true
                }

                Label {
                    text: "The next step is embedding libmpv here for local playback, seeking, subtitles, and audio track control."
                    color: "#9cb1c5"
                    wrapMode: Text.WordWrap
                    horizontalAlignment: Text.AlignHCenter
                    font.pixelSize: 22
                    Layout.fillWidth: true
                }

                Label {
                    text: appState.currentMediaPath
                    color: "#6d8194"
                    wrapMode: Text.WrapAnywhere
                    horizontalAlignment: Text.AlignHCenter
                    font.pixelSize: 16
                    Layout.fillWidth: true
                }

                RowLayout {
                    Layout.alignment: Qt.AlignHCenter
                    spacing: 12

                    Button {
                        text: "Save 12:34 Resume"
                        onClicked: appState.savePlaybackPosition(754000)
                    }

                    Button {
                        text: "Clear Resume"
                        onClicked: appState.clearResume(appState.currentMediaPath)
                    }
                }
            }
        }
    }
}
