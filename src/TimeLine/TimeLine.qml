import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Layouts 1.0

import arstudio 1.0

Column {
    id: column
    spacing: 5

    Slider {
        id: slider
        anchors {
            left: parent.left
            right: parent.right
        }
        minimumValue: g_SAggregator.min_frame
        maximumValue: g_SAggregator.max_frame
        stepSize: 1.0
        tickmarksEnabled: true
        updateValueWhileDragging: true
        value: spinbox.value
        onValueChanged: {
            if (g_SAggregator.valid_frame (value))
                g_SAggregator.signal_frame (value)
        }

    }

    Row {
        anchors {
            left: parent.left
            right: parent.right
        }
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
            id: spinbox
            anchors {
                top: parent.top
                bottom: parent.bottom
            }
            minimumValue: g_SAggregator.min_frame
            maximumValue: g_SAggregator.max_frame
            value: slider.value
        }
    }
}
