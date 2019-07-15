import QtQuick 2.12
import QtQuick.Controls 1.3
import QtQuick.Dialogs 1.2


Item {
    id: root
    property string label: "Custom Button"

    signal clicked()
    signal pressed()
    signal released()

    signal fileSelected(url file)

    Rectangle {
        color: "white"
        width: root.width
        height: root.height

        border {
            width: 1
            color: "black"
        }

        Text {
            anchors.centerIn: parent
            text: root.label
        }

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

        MouseArea {
            anchors.fill: parent

            onEntered: {
                parent.border.width = 2
                root.clicked()
            }
            onReleased: {
                parent.border.width = 1

                root.pressed()
                root.released()

                fileDialog.visible = true
            }
        }
    }
}
