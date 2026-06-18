import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ApplicationWindow {
    id: window
    visible: true
    width: 600
    height: 750
    title: "OS DropZone [TUI SIMULATOR]"
    color: "black"

    property int currentTab: 0

    // Custom Component for Retro Terminal Tab Buttons
    component TabButton : Button {
        property bool active: false
        id: tabBtn
        contentItem: Text {
            text: tabBtn.text
            color: tabBtn.active ? "black" : "#00FF00"
            font.family: "monospace"
            font.bold: true
            font.pixelSize: 13
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
        background: Rectangle {
            color: tabBtn.active ? "#00FF00" : "black"
            border.color: "#00FF00"
            border.width: 1
        }
    }

    // Keyboard Shortcuts for Navigation
    Shortcut { sequence: "F1"; onActivated: currentTab = 0 }
    Shortcut { sequence: "F2"; onActivated: currentTab = 1 }
    Shortcut { sequence: "F3"; onActivated: currentTab = 2 }
    Shortcut { sequence: "F4"; onActivated: currentTab = 3 }
    Shortcut { sequence: "F5"; onActivated: currentTab = 4 }
    Shortcut {
        sequence: "Tab"
        onActivated: currentTab = (currentTab + 1) % 5
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 15
        spacing: 12

        // --- SECTION 1: HEADER & LOGO ---
        Text {
            text: "┌──────────────────────────────────────────────────────────┐\n" +
                  "│   ___  ____   ___  ____   ___ ___   ___  _  _  ___       │\n" +
                  "│  / _ \\/ ___/ / _ \\/ ___/ / _ \\_  \\ / _ \\/ \\/ \\/ _ \\      │\n" +
                  "│  \\___/\\___ \\ \\___/\\___ \\ \\___/ /_/ \\___/_/\\_/\\___/      │\n" +
                  "│  C R O S S - O S   D R O P Z O N E   [ v1.0.0 ]          │\n" +
                  "└──────────────────────────────────────────────────────────┘"
            color: "#00FF00"
            font.family: "monospace"
            font.pixelSize: 11
            font.bold: true
            Layout.alignment: Qt.AlignHCenter
            horizontalAlignment: Text.AlignHCenter
        }

        // --- SECTION 2: OS SYSTEM METADATA ---
        Rectangle {
            Layout.fillWidth: true
            height: 35
            color: "black"
            border.color: "#00FF00"
            border.width: 1

            Text {
                text: "[ SYS: " + Backend.currentOS + " | NODE: " + Backend.deviceName + " | LAN: " + Backend.localIP + " ]"
                color: "#00FF00"
                font.family: "monospace"
                font.pixelSize: 12
                font.bold: true
                anchors.centerIn: parent
            }
        }

        // --- SECTION 3: TAB NAVIGATION ---
        RowLayout {
            Layout.fillWidth: true
            spacing: 3

            TabButton {
                text: "FEED (F1)"
                active: currentTab === 0
                Layout.fillWidth: true
                Layout.preferredHeight: 35
                onClicked: currentTab = 0
            }
            TabButton {
                text: "PEERS (F2)"
                active: currentTab === 1
                Layout.fillWidth: true
                Layout.preferredHeight: 35
                onClicked: currentTab = 1
            }
            TabButton {
                text: "KARCHARA (F3)"
                active: currentTab === 2
                Layout.fillWidth: true
                Layout.preferredHeight: 35
                onClicked: currentTab = 2
            }
            TabButton {
                text: "SETTINGS (F4)"
                active: currentTab === 3
                Layout.fillWidth: true
                Layout.preferredHeight: 35
                onClicked: currentTab = 3
            }
            TabButton {
                text: "HELP (F5)"
                active: currentTab === 4
                Layout.fillWidth: true
                Layout.preferredHeight: 35
                onClicked: currentTab = 4
            }
        }

        // --- SECTION 4: DYNAMIC BODY WORKSPACE ---
        StackLayout {
            currentIndex: currentTab
            Layout.fillWidth: true
            Layout.fillHeight: true

            // --- TAB 0: FEED LOG & WRITER ---
            ColumnLayout {
                spacing: 12

                // Log History Scroll Block
                Rectangle {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    color: "black"
                    border.color: "#00FF00"
                    border.width: 1

                    Text {
                        text: " ┌── SYSTEM LOG FEED ──┐ "
                        color: "#00FF00"
                        font.family: "monospace"
                        font.bold: true
                        anchors.top: parent.top
                        anchors.topMargin: -height / 2
                        anchors.horizontalCenter: parent.horizontalCenter
                        Rectangle { anchors.fill: parent; color: "black"; z: -1 }
                    }

                    ListView {
                        id: notesListView
                        anchors.fill: parent
                        anchors.margins: 12
                        spacing: 12
                        clip: true
                        model: Backend.notes

                        delegate: Rectangle {
                            width: notesListView.width - 24
                            implicitHeight: cardLayout.implicitHeight + 20
                            color: "black"
                            border.color: modelData.pinned ? "#00FF00" : "#004400"
                            border.width: modelData.pinned ? 2 : 1

                            ColumnLayout {
                                id: cardLayout
                                anchors.fill: parent
                                anchors.margins: 10
                                spacing: 8

                                RowLayout {
                                    Layout.fillWidth: true
                                    Text {
                                        text: (modelData.pinned ? "[* PINNED] " : "") + "[" + modelData.timestamp + " | OS: " + modelData.os + " | " + modelData.type + "]"
                                        color: modelData.pinned ? "#00FF00" : "#008800"
                                        font.family: "monospace"
                                        font.bold: modelData.pinned
                                        font.pixelSize: 11
                                    }
                                    Item { Layout.fillWidth: true }
                                }

                                Rectangle {
                                    Layout.fillWidth: true
                                    height: 1
                                    color: modelData.pinned ? "#00FF00" : "#002200"
                                }

                                Text {
                                    text: modelData.body
                                    color: "#00FF00"
                                    font.family: "monospace"
                                    font.pixelSize: 13
                                    wrapMode: Text.Wrap
                                    Layout.fillWidth: true
                                }

                                RowLayout {
                                    Layout.fillWidth: true
                                    spacing: 8
                                    Item { Layout.fillWidth: true }

                                    Button {
                                        id: copyBtn
                                        text: "[ COPY ]"
                                        Layout.preferredHeight: 25
                                        onClicked: Backend.copyToClipboard(modelData.body)
                                        contentItem: Text {
                                            text: copyBtn.text
                                            color: copyBtn.hovered ? "black" : "#00FF00"
                                            font.family: "monospace"
                                            font.pixelSize: 10
                                            font.bold: true
                                            horizontalAlignment: Text.AlignHCenter
                                            verticalAlignment: Text.AlignVCenter
                                        }
                                        background: Rectangle {
                                            color: copyBtn.hovered ? "#00FF00" : "black"
                                            border.color: "#00FF00"
                                            border.width: 1
                                        }
                                    }

                                    Button {
                                        id: pinBtn
                                        text: modelData.pinned ? "[ UNPIN ]" : "[ PIN ]"
                                        Layout.preferredHeight: 25
                                        onClicked: Backend.togglePin(modelData.index)
                                        contentItem: Text {
                                            text: pinBtn.text
                                            color: pinBtn.hovered ? "black" : "#00FF00"
                                            font.family: "monospace"
                                            font.pixelSize: 10
                                            font.bold: true
                                            horizontalAlignment: Text.AlignHCenter
                                            verticalAlignment: Text.AlignVCenter
                                        }
                                        background: Rectangle {
                                            color: pinBtn.hovered ? "#00FF00" : "black"
                                            border.color: "#00FF00"
                                            border.width: 1
                                        }
                                    }

                                    Button {
                                        id: delBtn
                                        text: "[ DELETE ]"
                                        Layout.preferredHeight: 25
                                        onClicked: Backend.deleteNote(modelData.index)
                                        contentItem: Text {
                                            text: delBtn.text
                                            color: delBtn.hovered ? "black" : "#FF3333"
                                            font.family: "monospace"
                                            font.pixelSize: 10
                                            font.bold: true
                                            horizontalAlignment: Text.AlignHCenter
                                            verticalAlignment: Text.AlignVCenter
                                        }
                                        background: Rectangle {
                                            color: delBtn.hovered ? "#FF3333" : "black"
                                            border.color: delBtn.hovered ? "#FF3333" : "#440000"
                                            border.width: 1
                                        }
                                    }
                                }
                            }
                        }
                    }
                }

                // Drag & Drop DropArea Overlay
                DropArea {
                    id: fileDropArea
                    anchors.fill: parent
                    onDropped: (drop) => {
                        if (drop.hasUrls) {
                            for (var i = 0; i < drop.urls.length; ++i) {
                                Backend.saveFileDrop(drop.urls[i])
                            }
                        }
                    }
                }

                // Note Input Text Area
                Rectangle {
                    Layout.fillWidth: true
                    height: 100
                    color: "black"
                    border.color: "#00FF00"
                    border.width: 1

                    Text {
                        text: " ┌── ENTER DROP MESSAGE (Ctrl+Enter to Save) ──┐ "
                        color: "#00FF00"
                        font.family: "monospace"
                        font.bold: true
                        anchors.top: parent.top
                        anchors.topMargin: -height / 2
                        anchors.left: parent.left
                        anchors.leftMargin: 15
                        Rectangle { anchors.fill: parent; color: "black"; z: -1 }
                    }

                    RowLayout {
                        anchors.fill: parent
                        anchors.margins: 12
                        spacing: 8

                        Text {
                            text: ">"
                            color: "#00FF00"
                            font.family: "monospace"
                            font.pixelSize: 14
                            font.bold: true
                            Layout.alignment: Qt.AlignTop
                            Layout.topMargin: 2
                        }

                        TextArea {
                            id: noteInput
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            color: "#00FF00"
                            font.family: "monospace"
                            font.pixelSize: 13
                            wrapMode: TextEdit.Wrap
                            placeholderText: "Type note, paste code, drop URL here..."
                            placeholderTextColor: "#004D00"
                            background: null

                            Keys.onPressed: (event) => {
                                if (event.key === Qt.Key_Return && (event.modifiers & Qt.ControlModifier)) {
                                    saveAction.clicked()
                                    event.accepted = true
                                }
                            }
                        }
                    }
                }

                // Control Buttons Action Bar
                RowLayout {
                    Layout.fillWidth: true
                    spacing: 8

                    Button {
                        id: saveAction
                        text: "[ SAVE NOTE ]"
                        Layout.fillWidth: true
                        Layout.preferredHeight: 35
                        onClicked: {
                            if (noteInput.text.trim() !== "") {
                                Backend.saveNote(noteInput.text.trim())
                                noteInput.text = ""
                            }
                        }
                        contentItem: Text {
                            text: saveAction.text
                            color: saveAction.hovered ? "black" : "#00FF00"
                            font.family: "monospace"
                            font.bold: true
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }
                        background: Rectangle {
                            color: saveAction.hovered ? "#00FF00" : "black"
                            border.color: "#00FF00"
                            border.width: 1
                        }
                    }

                    Button {
                        id: clearAction
                        text: "[ CLEAN LOG ]"
                        Layout.fillWidth: true
                        Layout.preferredHeight: 35
                        onClicked: Backend.clearHistory()
                        contentItem: Text {
                            text: clearAction.text
                            color: clearAction.hovered ? "black" : "#00FF00"
                            font.family: "monospace"
                            font.bold: true
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }
                        background: Rectangle {
                            color: clearAction.hovered ? "#00FF00" : "black"
                            border.color: "#00FF00"
                            border.width: 1
                        }
                    }

                    Button {
                        id: openAction
                        text: "[ EXPLORE FOLDER ]"
                        Layout.fillWidth: true
                        Layout.preferredHeight: 35
                        onClicked: Backend.openSharedFolder()
                        contentItem: Text {
                            text: openAction.text
                            color: openAction.hovered ? "black" : "#00FF00"
                            font.family: "monospace"
                            font.bold: true
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }
                        background: Rectangle {
                            color: openAction.hovered ? "#00FF00" : "black"
                            border.color: "#00FF00"
                            border.width: 1
                        }
                    }
                }
            }

            // --- TAB 1: LAN PEERS & SYNC ---
            ColumnLayout {
                spacing: 15

                Button {
                    id: toggleLanAction
                    text: Backend.lanSync ? "[ LAN SYNC: ACTIVATED ]" : "[ LAN SYNC: DEACTIVATED ]"
                    Layout.fillWidth: true
                    Layout.preferredHeight: 40
                    onClicked: Backend.lanSync = !Backend.lanSync
                    contentItem: Text {
                        text: toggleLanAction.text
                        color: toggleLanAction.hovered ? "black" : "#00FF00"
                        font.family: "monospace"
                        font.bold: true
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                    background: Rectangle {
                        color: toggleLanAction.hovered ? "#00FF00" : "black"
                        border.color: "#00FF00"
                        border.width: 1
                    }
                }

                Rectangle {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    color: "black"
                    border.color: "#00FF00"
                    border.width: 1

                    Text {
                        text: " ┌── ACTIVE LAN DETECTIONS ──┐ "
                        color: "#00FF00"
                        font.family: "monospace"
                        font.bold: true
                        anchors.top: parent.top
                        anchors.topMargin: -height / 2
                        anchors.horizontalCenter: parent.horizontalCenter
                        Rectangle { anchors.fill: parent; color: "black"; z: -1 }
                    }

                    ListView {
                        anchors.fill: parent
                        anchors.margins: 20
                        clip: true
                        model: Backend.peerList
                        delegate: Text {
                            text: "=> " + modelData
                            color: "#00FF00"
                            font.family: "monospace"
                            font.pixelSize: 13
                            height: 28
                        }
                    }
                }

                Text {
                    text: "* Continuous UDP broadcasts on port 40404 scan your local subnet for other running DropZone units."
                    color: "#004D00"
                    font.family: "monospace"
                    font.pixelSize: 11
                    wrapMode: Text.Wrap
                    Layout.fillWidth: true
                }
            }

            // --- TAB 2: KARCHARA (TRASH BIN) ---
            ColumnLayout {
                spacing: 12

                Rectangle {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    color: "black"
                    border.color: "#FF3333"
                    border.width: 1

                    Text {
                        text: " ┌── KARCHARA (TRASH BIN) ──┐ "
                        color: "#FF3333"
                        font.family: "monospace"
                        font.bold: true
                        anchors.top: parent.top
                        anchors.topMargin: -height / 2
                        anchors.horizontalCenter: parent.horizontalCenter
                        Rectangle { anchors.fill: parent; color: "black"; z: -1 }
                    }

                    ListView {
                        id: trashListView
                        anchors.fill: parent
                        anchors.margins: 12
                        spacing: 12
                        clip: true
                        model: Backend.trashNotes

                        Text {
                            text: "Karchara is empty. Clean space, clean mind!"
                            color: "#555555"
                            font.family: "monospace"
                            font.pixelSize: 13
                            anchors.centerIn: parent
                            visible: trashListView.count === 0
                        }

                        delegate: Rectangle {
                            width: trashListView.width - 24
                            implicitHeight: trashCardLayout.implicitHeight + 20
                            color: "black"
                            border.color: "#550000"
                            border.width: 1

                            ColumnLayout {
                                id: trashCardLayout
                                anchors.fill: parent
                                anchors.margins: 10
                                spacing: 8

                                RowLayout {
                                    Layout.fillWidth: true
                                    Text {
                                        text: "[DELETED: " + modelData.deletedAt + "] [" + modelData.timestamp + " | OS: " + modelData.os + "]"
                                        color: "#880000"
                                        font.family: "monospace"
                                        font.pixelSize: 11
                                    }
                                    Item { Layout.fillWidth: true }
                                }

                                Rectangle {
                                    Layout.fillWidth: true
                                    height: 1
                                    color: "#330000"
                                }

                                Text {
                                    text: modelData.body
                                    color: "#888888"
                                    font.family: "monospace"
                                    font.pixelSize: 13
                                    wrapMode: Text.Wrap
                                    Layout.fillWidth: true
                                }

                                RowLayout {
                                    Layout.fillWidth: true
                                    spacing: 8
                                    Item { Layout.fillWidth: true }

                                    Button {
                                        id: restoreBtn
                                        text: "[ RESTORE ]"
                                        Layout.preferredHeight: 25
                                        onClicked: Backend.restoreNote(modelData.index)
                                        contentItem: Text {
                                            text: restoreBtn.text
                                            color: restoreBtn.hovered ? "black" : "#00FF00"
                                            font.family: "monospace"
                                            font.pixelSize: 10
                                            font.bold: true
                                            horizontalAlignment: Text.AlignHCenter
                                            verticalAlignment: Text.AlignVCenter
                                        }
                                        background: Rectangle {
                                            color: restoreBtn.hovered ? "#00FF00" : "black"
                                            border.color: "#00FF00"
                                            border.width: 1
                                        }
                                    }

                                    Button {
                                        id: delForeverBtn
                                        text: "[ DELETE FOREVER ]"
                                        Layout.preferredHeight: 25
                                        onClicked: Backend.deleteNoteForever(modelData.index)
                                        contentItem: Text {
                                            text: delForeverBtn.text
                                            color: delForeverBtn.hovered ? "black" : "#FF3333"
                                            font.family: "monospace"
                                            font.pixelSize: 10
                                            font.bold: true
                                            horizontalAlignment: Text.AlignHCenter
                                            verticalAlignment: Text.AlignVCenter
                                        }
                                        background: Rectangle {
                                            color: delForeverBtn.hovered ? "#FF3333" : "black"
                                            border.color: delForeverBtn.hovered ? "#FF3333" : "#440000"
                                            border.width: 1
                                        }
                                    }
                                }
                            }
                        }
                    }
                }

                Text {
                    text: "* Items in Karchara are kept for 1 week before being deleted permanently."
                    color: "#550000"
                    font.family: "monospace"
                    font.pixelSize: 11
                    wrapMode: Text.Wrap
                    Layout.fillWidth: true
                }
            }

            // --- TAB 3: PATH & CLIPBOARD SETTINGS ---
            ColumnLayout {
                spacing: 15

                Button {
                    id: toggleClipboardAction
                    text: Backend.clipboardSync ? "[ CLIPBOARD SYNC: AUTOMATIC MONITORING ON ]" : "[ CLIPBOARD SYNC: DEACTIVATED ]"
                    Layout.fillWidth: true
                    Layout.preferredHeight: 40
                    onClicked: Backend.clipboardSync = !Backend.clipboardSync
                    contentItem: Text {
                        text: toggleClipboardAction.text
                        color: toggleClipboardAction.hovered ? "black" : "#00FF00"
                        font.family: "monospace"
                        font.bold: true
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                    background: Rectangle {
                        color: toggleClipboardAction.hovered ? "#00FF00" : "black"
                        border.color: "#00FF00"
                        border.width: 1
                    }
                }

                Rectangle {
                    Layout.fillWidth: true
                    height: 70
                    color: "black"
                    border.color: "#00FF00"
                    border.width: 1

                    Text {
                        text: " ┌── SHARED WRITING PATH (Double click to edit) ──┐ "
                        color: "#00FF00"
                        font.family: "monospace"
                        font.bold: true
                        anchors.top: parent.top
                        anchors.topMargin: -height / 2
                        anchors.left: parent.left
                        anchors.leftMargin: 15
                        Rectangle { anchors.fill: parent; color: "black"; z: -1 }
                    }

                    TextInput {
                        id: pathInputField
                        anchors.fill: parent
                        anchors.margins: 15
                        text: Backend.sharedPath
                        color: "#00FF00"
                        font.family: "monospace"
                        font.pixelSize: 13
                        selectByMouse: true
                        verticalAlignment: Text.AlignVCenter
                    }
                }

                Button {
                    id: applyPathAction
                    text: "[ APPLY PATH DIRECTORY ]"
                    Layout.fillWidth: true
                    Layout.preferredHeight: 35
                    onClicked: {
                        Backend.sharedPath = pathInputField.text
                    }
                    contentItem: Text {
                        text: applyPathAction.text
                        color: applyPathAction.hovered ? "black" : "#00FF00"
                        font.family: "monospace"
                        font.bold: true
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                    background: Rectangle {
                        color: applyPathAction.hovered ? "#00FF00" : "black"
                        border.color: "#00FF00"
                        border.width: 1
                    }
                }

                Item { Layout.fillHeight: true }
            }

            // --- TAB 4: HELP & KNOWLEDGE BASE ---
            ColumnLayout {
                spacing: 15

                Rectangle {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    color: "black"
                    border.color: "#00FF00"
                    border.width: 1

                    Text {
                        text: " ┌── DUAL-BOOT STORAGE INSTRUCTIONS ──┐ "
                        color: "#00FF00"
                        font.family: "monospace"
                        font.bold: true
                        anchors.top: parent.top
                        anchors.topMargin: -height / 2
                        anchors.horizontalCenter: parent.horizontalCenter
                        Rectangle { anchors.fill: parent; color: "black"; z: -1 }
                    }

                    ScrollView {
                        anchors.fill: parent
                        anchors.margins: 15
                        clip: true

                        Text {
                            text: "To share notes/links sequentially on dual-boot machines:\n\n" +
                                  "1. Map/create a shared partition (NTFS or exFAT works best).\n" +
                                  "2. Mount the partition under both systems:\n" +
                                  "   * Linux: /mnt/shared_data/\n" +
                                  "   * Windows: D:/shared_data/\n" +
                                  "3. Open settings (F3) and sync paths to point to:\n" +
                                  "   * /mnt/shared_data/shared_notes.txt  (on Linux)\n" +
                                  "   * D:/shared_data/shared_notes.txt     (on Windows)\n\n" +
                                  "----------------------------------------------------\n\n" +
                                  "To share clipboard logs live over network (Concurrent):\n\n" +
                                  "1. Power on both machines and connect to the same LAN.\n" +
                                  "2. Open the Peers panel (F2) on both devices.\n" +
                                  "3. Enable LAN SYNC.\n" +
                                  "4. Enable CLIPBOARD SYNC in Settings (F3).\n" +
                                  "5. Text copied on one OS is instantly mirrored to the other."
                            color: "#00FF00"
                            font.family: "monospace"
                            font.pixelSize: 12
                            wrapMode: Text.Wrap
                            width: parent.width
                        }
                    }
                }
            }
        }

        // --- SECTION 5: FOOTER ACTION CHEATSHEET ---
        Rectangle {
            Layout.fillWidth: true
            height: 30
            color: "black"
            border.color: "#00FF00"
            border.width: 1

            Text {
                text: " [TAB] Cycle Views │ F1 Feed │ F2 Peers │ F3 Karchara │ F4 Settings │ F5 Help "
                color: "#00FF00"
                font.family: "monospace"
                font.pixelSize: 12
                font.bold: true
                anchors.centerIn: parent
            }
        }
    }

    // Drag-over visual prompt
    Rectangle {
        anchors.fill: parent
        color: "black"
        opacity: fileDropArea.containsDrag ? 0.92 : 0
        visible: opacity > 0
        border.color: "#00FF00"
        border.width: 2

        ColumnLayout {
            anchors.centerIn: parent
            spacing: 20

            Text {
                text: "┌──────────────────────────────────────────────┐\n" +
                      "│             >>> FILE DETECTED <<<            │\n" +
                      "│                                              │\n" +
                      "│      RELEASE TO LOG AND STAGE COPY TO:       │\n" +
                      "│      ~/shared_notes_directory/drops/         │\n" +
                      "│                                              │\n" +
                      "└──────────────────────────────────────────────┘"
                color: "#00FF00"
                font.family: "monospace"
                font.pixelSize: 15
                font.bold: true
                horizontalAlignment: Text.AlignHCenter
            }
        }

        Behavior on opacity {
            NumberAnimation { duration: 150 }
        }
    }
}