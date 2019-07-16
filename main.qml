import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Dialogs 1.2
import QtQuick.Scene3D 2.0
import QtQuick.Dialogs 1.2


Item {
    Rectangle {
        id: scene
        anchors.fill: parent
        anchors.leftMargin: 5
        anchors.rightMargin: 5
        anchors.bottomMargin: button.height * 1.25
        color: "white"

        Scene3D {
            id: scene3d
            anchors.fill: parent
            anchors.margins: 10
            focus: true
            aspects: ["input", "logic"]
            cameraAspectRatioMode: Scene3D.AutomaticAspectRatio

            Scene {}
        }
    }

    CoordinateInput {
        id: coord_inp
        label: "Enter Coordinate"
        width: 150
        height: 105
        visible: false
    }

    Button {
        id: button
        label: "Load Mesh"

        width: 100
        height: 35
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.margins: 5

        FileDialog {
            id: fileDialog
            title: "Please choose a file"
            folder: shortcuts.home
            onAccepted: {
                console.log("You chose: " + fileDialog.fileUrls)
                root.fileSelected(fileDialog.fileUrl)
                Qt.quit()
            }
            onRejected: {
                console.log("Canceled")
                Qt.quit()
            }
        }

        onReleased: {
            fileDialog.visible = true
        }
    }
}