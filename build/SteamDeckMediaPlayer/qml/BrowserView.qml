import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

FocusScope {
    id: root

    Rectangle {
        anchors.fill: parent
        gradient: Gradient {
            GradientStop { position: 0.0; color: "#162433" }
            GradientStop { position: 1.0; color: "#0b1016" }
        }
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 32
        spacing: 20

        RowLayout {
            Layout.fillWidth: true
            spacing: 16

            Label {
                text: "Browse"
                color: "#f4f7fb"
                font.pixelSize: 34
                font.weight: Font.DemiBold
            }

            Item {
                Layout.fillWidth: true
            }

            CheckBox {
                text: "Show hidden"
                checked: appState.showHidden
                onToggled: appState.showHidden = checked
            }
        }

        Rectangle {
            Layout.fillWidth: true
            implicitHeight: 60
            radius: 16
            color: "#182635"
            border.width: 1
            border.color: "#31465b"

            RowLayout {
                anchors.fill: parent
                anchors.leftMargin: 20
                anchors.rightMargin: 20

                Label {
                    text: appState.browserModel.currentPath
                    color: "#ced9e5"
                    elide: Text.ElideMiddle
                    Layout.fillWidth: true
                    font.pixelSize: 22
                }

                Button {
                    text: "Up"
                    enabled: appState.browserModel.canGoUp()
                    onClicked: appState.navigateUp()
                }
            }
        }

        ListView {
            id: browserList
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true
            model: appState.browserModel
            focus: true
            currentIndex: count > 0 ? Math.max(0, currentIndex) : -1
            keyNavigationWraps: false
            boundsBehavior: Flickable.StopAtBounds
            spacing: 10

            Keys.onPressed: event => {
                if (event.key === Qt.Key_Return || event.key === Qt.Key_Enter || event.key === Qt.Key_Space) {
                    if (currentIndex >= 0 && currentItem) {
                        appState.openEntry(currentItem.path, currentItem.isDirectory, currentItem.isPlayable)
                    }
                    event.accepted = true
                } else if (event.key === Qt.Key_Backspace || event.key === Qt.Key_Escape) {
                    appState.goBack()
                    event.accepted = true
                }
            }

            delegate: Rectangle {
                required property string name
                required property string path
                required property bool isDirectory
                required property bool isPlayable

                width: browserList.width
                height: 72
                radius: 18
                color: ListView.isCurrentItem ? "#3f8cff" : "#162433"
                border.width: 1
                border.color: ListView.isCurrentItem ? "#90bbff" : "#243443"

                RowLayout {
                    anchors.fill: parent
                    anchors.leftMargin: 22
                    anchors.rightMargin: 22
                    spacing: 18

                    Label {
                        text: isDirectory ? "\u25b8" : "\u25ba"
                        color: ListView.isCurrentItem ? "#0b1016" : "#88c6ff"
                        font.pixelSize: 22
                    }

                    ColumnLayout {
                        Layout.fillWidth: true
                        spacing: 2

                        Label {
                            text: name
                            color: ListView.isCurrentItem ? "#08111b" : "#f1f5fa"
                            font.pixelSize: 24
                            elide: Text.ElideRight
                            Layout.fillWidth: true
                        }

                        Label {
                            text: isDirectory ? "Folder" : "Media file"
                            color: ListView.isCurrentItem ? "#16304d" : "#93a7bb"
                            font.pixelSize: 16
                        }
                    }
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        browserList.currentIndex = index
                        appState.openEntry(path, isDirectory, isPlayable)
                    }
                }
            }

            ScrollBar.vertical: ScrollBar {}
        }

        Label {
            text: "Controls: D-pad or arrows move, A/Enter opens, B/Escape goes back"
            color: "#93a7bb"
            font.pixelSize: 18
        }
    }
}
