import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Window 2.1
import QtQuick.Layouts 1.0

import arstudio 1.0

/**
  This component implements the processing dialog, allowing the user
  to select processing options, begin processing and monitor it's progress
*/

Window {
    property url sourceFile
    property alias video_helper: vh

    title: "Processing Dialog"
    flags: Qt.Dialog
    modality: Qt.WindowModal
    color: g_systemPalette.window
    width: layout.childrenRect.width
    height: layout.childrenRect.height

    onSourceFileChanged: show()

    ExclusiveGroup { id: frameChoiceGroup }

    ColumnLayout {
        id: layout
        anchors.fill: parent

        Label { text : "Selected file: <b>" + vh.source_basename + "</b>" }

        RadioButton {
            id: wholeFile
            text: "Process whole file"
            exclusiveGroup: frameChoiceGroup
            checked: true
            enabled: !vp.running
        }

        RowLayout {
            RadioButton {
                id: selectFrames
                text: "Select frames: "
                exclusiveGroup: frameChoiceGroup
                enabled: !vp.running
            }

            SpinBox {
                id: min
                minimumValue: 1
                maximumValue: max.value
                value: 1
                Layout.minimumWidth: 50
                enabled: selectFrames.checked
            }

            SpinBox {
                id: max
                minimumValue: min.value
                maximumValue: vh.frame_count
                value: 1
                Layout.minimumWidth: 50
                enabled: selectFrames.checked
            }
        }

        RowLayout {
            ProgressBar {
                id: pb
                value: vp.progress
            }

            Button {
                text: "Stop"
                onClicked: vp.running = false
                enabled: vp.running
            }
        }

        RowLayout {
            Button {
                text: "Process"
                enabled: vh.status && !vp.running
                onClicked: vp.running = true
            }

            Button {
                text: "Cancel"
                onClicked: vp.running = false
                enabled: !vp.running
            }
        }
    }

    VideoHelper {
        id: vh
        source: sourceFile
    }

    VideoPipeline {
        id: vp
        video_helper: vh
        config: g_Config
        start_frame: wholeFile.checked ? 1 : min.value
        end_frame: wholeFile.checked ? vh.frame_count : max.value
        onRunningChanged: {
            if (!running)
                hide ()
        }
    }
}
