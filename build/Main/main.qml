import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts

ApplicationWindow {
    visible: true
    width: 450
    height: 650
    title: "OS DropZone"
    
    Material.theme: Material.Dark
    Material.accent: Material.Purple

    // Run this when the app first opens to load past history
    Component.onCompleted: {
        historyDisplay.text = Backend.loadHistory()
    }

    Rectangle {
        anchors.fill: parent
        color: "#121212" // Deep dark mode

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 20
            spacing: 15

            // --- THE INPUT AREA ---
            TextArea {
                id: noteInput
                Layout.fillWidth: true
                Layout.preferredHeight: 120
                placeholderText: "Drop a link, thought, or snippet here..."
                wrapMode: TextEdit.Wrap
                
                background: Rectangle {
                    color: "#1e1e1e"
                    radius: 8
                    border.color: "#333333"
                }
            }

            Button {
                text: "Save Note"
                Layout.fillWidth: true
                Layout.preferredHeight: 45
                onClicked: {
                    if (noteInput.text.trim() !== "") {
                        Backend.saveNote(noteInput.text)
                        noteInput.text = "" // Clear the box
                        historyDisplay.text = Backend.loadHistory() // Refresh history!
                    }
                }
            }

            // --- THE HISTORY VIEWER ---
            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                color: "#1e1e1e"
                radius: 8
                border.color: "#333333"
                clip: true // Keeps text inside the box

                ScrollView {
                    anchors.fill: parent
                    anchors.margins: 10

                    TextArea {
                        id: historyDisplay
                        readOnly: true
                        wrapMode: TextEdit.Wrap
                        font.family: "monospace" // Good for code snippets
                        color: "#cccccc"
                        background: null // Remove default background
                    }
                }
            }
        }
    }
}