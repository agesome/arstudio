import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Layouts 1.0

Column {
    id: column

    property alias slider: slider

    spacing: 5

    Slider {
        id: slider

        anchors.left: parent.left
        anchors.right: parent.right

        minimumValue: 1
        maximumValue: 100
        stepSize: 1.0

        tickmarksEnabled: true
        updateValueWhileDragging: true
    }

    Row {
        anchors.left: parent.left
        anchors.right: parent.right
        spacing: parent.spacing

        Button {
            text: "<"
        }
        Button {
            text: ">"
        }
        Button {
            text: "Play"
        }
        Button {
            text: "Stop"
        }
        SpinBox {
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            value: slider.value
        }
    }
}
