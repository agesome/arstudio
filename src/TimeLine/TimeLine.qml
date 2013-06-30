import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Layouts 1.0

import arstudio 1.0

/**
  This component provides player-like controls for navigating Sequence frames
*/

ColumnLayout {
    Layout.fillHeight: false
    Layout.minimumHeight: slider.childrenRect.height
                          + rowlayout.childrenRect.height + 25

    Timer {
        id: timer
        interval: 350
        triggeredOnStart: true
        repeat: true
        onTriggered: {
            slider.value++
            if (slider.value === slider.maximumValue)
                running = false
        }
    }

    Binding {
        target: slider
        property: "value"
        value: spinbox.value
    }

    Binding {
        target: spinbox
        property: "value"
        value: slider.value
    }

    Slider {
        id: slider
        Layout.fillWidth: true

        minimumValue: SAggregator.min_frame
        maximumValue: SAggregator.max_frame
        value: 1

        stepSize: 1.0
        tickmarksEnabled: true

        onValueChanged: SAggregator.signal_frame(value)
    }

    RowLayout {
        id: rowlayout
        Layout.fillHeight: false

        Button {
            Layout.minimumWidth: height
            iconName: "media-skip-backward"
            onClicked: {
                if (slider.value - 1 >= slider.minimumValue)
                    slider.value--
            }
        }
        Button {
            Layout.minimumWidth: height
            iconName: "media-skip-forward"
            onClicked: {
                if (slider.value + 1 <= slider.maximumValue)
                    slider.value++
            }
        }
        Button {
            Layout.minimumWidth: height
            iconName: "media-playback-start"
            onClicked: timer.start()
        }
        Button {
            Layout.minimumWidth: height
            iconName: "media-playback-stop"
            onClicked: timer.stop()
        }
        SpinBox {
            id: spinbox
            Layout.minimumHeight: parent.height
            Layout.minimumWidth: 80
            minimumValue: SAggregator.min_frame
            maximumValue: SAggregator.max_frame
            value: 1
        }
    }
}
