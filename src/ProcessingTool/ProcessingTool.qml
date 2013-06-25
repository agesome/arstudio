import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Window 2.1
import QtQuick.Layouts 1.0
import QtQuick.Dialogs 1.0

import arstudio 1.0

/**
  This component implements the processing dialog, allowing the user
  to select processing options, begin processing and monitor it's progress
*/

Rectangle {
    anchors.fill: parent
    color: g_systemPalette.window

    ColumnLayout {
        anchors.fill: parent

        ExclusiveGroup { id: frameChoiceGroup }

        RowLayout {
            Label {
                Layout.fillWidth: true
                text: "Selected file: <b>" + videoHelper.source_basename
                      + "</b>"
            }
            Button {
                Layout.minimumWidth: height
                iconName: "document-open"
                onClicked: openFileDialog.visible = true
            }
        }

        Label {
            text: "Total frames: " + videoHelper.frame_count
        }

        RadioButton {
            id: wholeFile
            text: "Process whole file"
            exclusiveGroup: frameChoiceGroup
            checked: true
            enabled: !videoPipeline.running
        }

        RowLayout {
            RadioButton {
                id: selectFrames
                text: "Select frames: "
                exclusiveGroup: frameChoiceGroup
                enabled: !videoPipeline.running
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
                maximumValue: videoHelper.frame_count
                value: 1
                Layout.minimumWidth: 50
                enabled: selectFrames.checked
            }
        }

        RowLayout {
            ProgressBar {
                Layout.fillWidth: true
                Layout.minimumHeight: parent.height
                value: videoPipeline.progress
            }

            Button {
                id: processButton
                text: "Process"
                Layout.minimumWidth: 40
                enabled: videoHelper.status && !videoPipeline.running
                onClicked: videoPipeline.running = true
            }

            Button {
                text: "Stop"
                onClicked: videoPipeline.running = false
                enabled: videoPipeline.running
            }
        }

        VideoHelper {
            id: videoHelper
            source: openFileDialog.fileUrl
        }

        VideoPipeline {
            id: videoPipeline
            video_helper: videoHelper
            config: g_Config
            start_frame: wholeFile.checked ? 1 : min.value
            end_frame: wholeFile.checked ? videoHelper.frame_count : max.value
        }

        FileDialog {
            id: openFileDialog
            title: "Please choose a file"
            selectMultiple: false
            selectExisting: true
            selectFolder: false
            nameFilters: [ "Kinvideo files(*.kinvideo)", "All files(*.*)" ]
        }
    }
}
