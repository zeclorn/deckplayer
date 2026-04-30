import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    id: root
    property string mediaTitle: ""
    property string resumePositionLabel: ""

    signal resumeRequested()
    signal startOverRequested()
    signal cancelled()

    width: 460
    height: 260
    radius: 24
    color: "#15202b"
    border.width: 1
    border.color: "#3d5368"

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 28
        spacing: 18

        Label {
            text: "Resume playback?"
            color: "#f4f7fb"
            font.pixelSize: 28
            font.weight: Font.DemiBold
        }

        Label {
            text: mediaTitle
            color: "#d6e0ea"
            elide: Text.ElideRight
            Layout.fillWidth: true
            font.pixelSize: 20
        }

        Label {
            text: "Saved position: " + resumePositionLabel
            color: "#93a7bb"
            font.pixelSize: 18
        }

        Item {
            Layout.fillHeight: true
        }

        RowLayout {
            Layout.fillWidth: true
            spacing: 12

            Button {
                Layout.fillWidth: true
                text: "Resume"
                onClicked: root.resumeRequested()
            }

            Button {
                Layout.fillWidth: true
                text: "Start Over"
                onClicked: root.startOverRequested()
            }

            Button {
                Layout.fillWidth: true
                text: "Cancel"
                onClicked: root.cancelled()
            }
        }
    }
}
