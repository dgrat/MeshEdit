import QtQuick 2.12


Item {
    id: root
    property string label: "Custom Input"

    signal clicked()
    signal pressed()
    signal released()

    Rectangle {
        color: "white"
        width: parent.width
        height: inp_label.contentHeight*1.25 + inp_x_lab.contentHeight + inp_y_lab.contentHeight + inp_z_lab.contentHeight + 5 + ok_but.height + 5

        border {
            width: 1
            color: "black"
        }

        Button {
            id: ok_but
            label: "OK"
            width: 60
            height: 25
            anchors.bottom: parent.bottom
            anchors.left: parent.left
        }
        Button {
            id: cancel_but
            label: "Cancel"
            width: 60
            height: 25
            anchors.bottom: parent.bottom
            anchors.right: parent.right
        }

        Text {
            id: inp_label
            text: root.label
            y: 0
            font.pointSize: 12
        }

        Text {
            id: inp_x_lab
            text: qsTr("[x]=")
            x: root.x
            y: inp_label.contentHeight*1.25
            font.pointSize: 10
        }
        Text {
            id: inp_y_lab
            text: qsTr("[y]=")
            x: root.x
            y: inp_label.contentHeight*1.25 + inp_x_lab.contentHeight
            font.pointSize: 10
        }
        Text {
            id: inp_z_lab
            text: qsTr("[z]=")
            x: root.x
            y: inp_label.contentHeight*1.25 + inp_x_lab.contentHeight + inp_y_lab.contentHeight
            font.pointSize: 10
        }

        Rectangle {
            x: inp_x_lab.contentWidth + 5
            y: inp_x_lab.y
            width: root.width - (inp_x_lab.contentWidth + 10)
            height: inp_x_lab.contentHeight

            border {
                color: "black"
                width: 1
            }

            TextInput {
                id: inp_x
                x: root.x+5
                y: root.y
                width: root.width-5
                height: root.height
                inputMethodHints: Qt.ImhFormattedNumbersOnly
            }
        }

        Rectangle {
            x: inp_x_lab.contentWidth + 5
            y: inp_y_lab.y
            width: root.width - (inp_x_lab.contentWidth + 10)
            height: inp_y_lab.contentHeight

            border {
                color: "black"
                width: 1
            }

            TextInput {
                id: inp_y
                x: root.x+5
                y: root.y
                width: root.width-5
                height: root.height
                inputMethodHints: Qt.ImhFormattedNumbersOnly
            }
        }

        Rectangle {
            x: inp_x_lab.contentWidth + 5
            y: inp_z_lab.y
            width: root.width - (inp_x_lab.contentWidth + 10)
            height: inp_z_lab.contentHeight

            border {
                color: "black"
                width: 1
            }

            TextInput {
                id: inp_z
                x: root.x+5
                y: root.y
                width: root.width-5
                height: root.height
                inputMethodHints: Qt.ImhFormattedNumbersOnly
            }
        }
    }
}
