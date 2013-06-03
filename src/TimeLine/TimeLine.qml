import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Layouts 1.0

import arstudio 1.0

ColumnLayout {
    Layout.fillHeight: false

    Slider {
        id: slider
        Layout.fillWidth: true

        minimumValue: SAggregator.min_frame
        maximumValue: SAggregator.max_frame
        value: spinbox.value

        stepSize: 1.0
        tickmarksEnabled: true
        updateValueWhileDragging: true

        onValueChanged: {
            if (SAggregator.valid_frame (value))
                SAggregator.signal_frame (value)
        }
    }

    RowLayout {
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
            Layout.minimumHeight: parent.height
            Layout.minimumWidth: 80
            minimumValue: SAggregator.min_frame
            maximumValue: SAggregator.max_frame
            value: slider.value
        }
    }
}
