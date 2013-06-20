import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Layouts 1.0

import arstudio 1.0
import WindowTool 1.0

/**
  This component is responsible for displaying a list of sequences
  available for display in an ItemWindow and for adding user-selected
  sequences to ItemWindow's Scenegraph.
*/

RowLayout {
    id: root
    property int exclusiveSequenceIndex

    readonly property ItemWindow window: itemWindow
    property Scenegraph scenegraph: itemWindow.manager.scenegraph
    property alias windowManager: itemWindow.manager
    property alias title: itemWindow.title

    property alias skyboxSource: itemWindow.skyboxSource

    signal loadSkybox(Item handler)
    signal loadModel()

    ItemWindow {
        id: itemWindow
        width: 450
        height: 450
        onModelChanged: {
            modelControls.reset(position, rotation)
        }
        cameraView: cameraView.checked
    }

    TableView {
        id: tableView
        Layout.fillHeight: true
        Layout.fillWidth: true
        model: g_Repository

        itemDelegate: CheckBox {
            anchors.fill: parent
            text: styleData.value
            enabled: {
                if (!scenegraph)
                    return false

                var type = model.get(styleData.row).type
                if (scenegraph.locked_to === Scenegraph.BITMAP)
                    return (type === Sequence.BITMAP)
                            && (styleData.row === exclusiveSequenceIndex)
                else if (scenegraph.locked_to === Scenegraph.NORMAL)
                    return type !== Sequence.BITMAP

                return true
            }

            onCheckedChanged: {
                var sequence = model.get(styleData.row).ptr
                if (checked)
                    scenegraph.add_sequence(sequence)
                else
                    scenegraph.remove_sequence(sequence)

                if (scenegraph.locked_to === Scenegraph.BITMAP)
                    exclusiveSequenceIndex = styleData.row
            }

            MouseArea {
                anchors.fill: parent
                onClicked: checked = !checked
            }
        }

        TableViewColumn {
            role: "name"
            title: "Repository"
            width: tableView.width - 20
        }

        TableViewColumn {
            role: "type"
            width: 16
            delegate: Image {
                fillMode: Image.Pad
                source: switch (styleData.value) {
                        case Sequence.CAMERA: return "qrc:camera-photo.png"
                        case Sequence.BITMAP: return "qrc:emblem-photos.png"
                        }
            }
        }
    }

    ColumnLayout {
        Layout.alignment: Qt.AlignTop
        ModelControls {
            id: modelControls
            enabled: windowManager.selected_model
            onPositionChanged: window.selectedModel.position = position
            onRotationChanged: window.selectedModel.rotation = rotation
            Layout.alignment: Qt.AlignTop
        }

        RowLayout {
            Button {
                text: "Load Skybox"
                onClicked: loadSkybox(root)
            }
            Button {
                text: "Load Model"
                onClicked: loadModel()
            }
        }
        CheckBox {
            id: cameraView
            text: "Camera View"
            checked: false
        }
    }
}

