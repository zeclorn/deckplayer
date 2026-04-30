import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import SteamDeckMediaPlayer

FocusScope {
    id: root
    focus: true

    function activateCurrentEntry() {
        if (!browserList.currentItem) {
            return
        }

        AppState.openEntry(
            browserList.currentItem.path,
            browserList.currentItem.isDirectory,
            browserList.currentItem.isPlayable
        )
    }

    function ensureListFocus() {
        browserList.forceActiveFocus()
        if (browserList.count > 0 && browserList.currentIndex < 0) {
            browserList.currentIndex = 0
            browserList.positionViewAtBeginning()
        }
    }

    function moveSelection(delta) {
        if (browserList.count <= 0) {
            return
        }

        const nextIndex = Math.max(0, Math.min(browserList.count - 1, browserList.currentIndex + delta))
        browserList.currentIndex = nextIndex
        browserList.positionViewAtIndex(nextIndex, ListView.Contain)
    }

    Component.onCompleted: ensureListFocus()
    onVisibleChanged: if (visible) ensureListFocus()

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
                checked: AppState.showHidden
                onToggled: AppState.showHidden = checked
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
                    text: AppState.browserModel.currentPath
                    color: "#ced9e5"
                    elide: Text.ElideMiddle
                    Layout.fillWidth: true
                    font.pixelSize: 22
                }

                Button {
                    text: "Up"
                    enabled: AppState.browserModel.canGoUp()
                    onClicked: AppState.navigateUp()
                }
            }
        }

        Connections {
            target: AppState.browserModel

            function onCurrentPathChanged() {
                browserList.currentIndex = browserList.count > 0 ? 0 : -1
                browserList.positionViewAtBeginning()
                root.ensureListFocus()
            }
        }

        ListView {
            id: browserList
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true
            model: AppState.browserModel
            focus: true
            currentIndex: -1
            keyNavigationWraps: false
            boundsBehavior: Flickable.StopAtBounds
            spacing: 10

            onCountChanged: {
                if (count > 0 && currentIndex < 0) {
                    currentIndex = 0
                } else if (count === 0) {
                    currentIndex = -1
                }
            }

            Keys.onUpPressed: event => {
                root.moveSelection(-1)
                event.accepted = true
            }

            Keys.onDownPressed: event => {
                root.moveSelection(1)
                event.accepted = true
            }

            Keys.onPressed: event => {
                if (event.key === Qt.Key_Return || event.key === Qt.Key_Enter
                        || event.key === Qt.Key_Space || event.key === Qt.Key_Right
                        || event.key === Qt.Key_D) {
                    root.activateCurrentEntry()
                    event.accepted = true
                } else if (event.key === Qt.Key_Backspace || event.key === Qt.Key_Escape
                        || event.key === Qt.Key_Left || event.key === Qt.Key_A) {
                    AppState.goBack()
                    event.accepted = true
                } else if (event.key === Qt.Key_PageDown) {
                    root.moveSelection(10)
                    event.accepted = true
                } else if (event.key === Qt.Key_PageUp) {
                    root.moveSelection(-10)
                    event.accepted = true
                } else if (event.key === Qt.Key_Q) {
                    root.moveSelection(-8)
                    event.accepted = true
                } else if (event.key === Qt.Key_E) {
                    root.moveSelection(8)
                    event.accepted = true
                } else if (event.key === Qt.Key_W) {
                    root.moveSelection(-1)
                    event.accepted = true
                } else if (event.key === Qt.Key_S) {
                    root.moveSelection(1)
                    event.accepted = true
                } else if (event.key === Qt.Key_Home && count > 0) {
                    currentIndex = 0
                    positionViewAtBeginning()
                    event.accepted = true
                } else if (event.key === Qt.Key_End && count > 0) {
                    currentIndex = count - 1
                    positionViewAtEnd()
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
                    onPressed: {
                        browserList.currentIndex = index
                        browserList.forceActiveFocus()
                    }
                    onClicked: {
                        browserList.currentIndex = index
                        browserList.forceActiveFocus()
                        AppState.openEntry(path, isDirectory, isPlayable)
                    }
                }
            }

            ScrollBar.vertical: ScrollBar {}
        }

        Label {
            text: "Controls: D-pad/arrows or W/S move, A/Enter/Right/D opens, B/Escape/Left/A goes back, Q/E or Page Up/Down jump"
            color: "#93a7bb"
            font.pixelSize: 18
        }
    }
}
