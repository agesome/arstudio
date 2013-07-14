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
            modelControls.reset(position, rotation, scale)
        }
        cameraView: cameraView.checked
        showModelIndicator: showModelIndicator.checked
    }

    TableView {
        id: tableView
        Layout.fillHeight: true
        Layout.fillWidth: true
        model: g_Repository

        TableViewColumn {
            role: "name"
            title: "Repository"
            width: tableView.width - 20
            delegate: CheckBox {
                anchors.fill: parent
                text: styleData.value
                enabled: {
                    if (!scenegraph || model.nodes.length === 0)
                        return false

                    var type = model.nodes[styleData.row].type
                    if (scenegraph.locked_to === Scenegraph.BITMAP)
                        return (type === Sequence.Bitmap)
                                && (styleData.row === exclusiveSequenceIndex)
                    else if (scenegraph.locked_to === Scenegraph.NORMAL)
                        return type !== Sequence.Bitmap

                    return true
                }

                onCheckedChanged: {
                    var sequence = model.nodes[styleData.row].ptr
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
        }

        TableViewColumn {
            role: "type"
            width: 16
            delegate: Image {
                fillMode: Image.Pad
                source: switch (styleData.value) {
                        case Sequence.Camera: return "qrc:camera-photo.png"
                        case Sequence.Bitmap: return "qrc:emblem-photos.png"
                        }
            }
        }
    }

    ColumnLayout {
        Layout.alignment: Qt.AlignTop
        ModelControls {
            Layout.fillWidth: true
            id: modelControls
            enabled: windowManager.selected_model
            onPositionChanged: window.selectedModel.position = position
            onRotationChanged: window.selectedModel.rotation = rotation
            onScaleChanged: window.selectedModel.scale = scale
            Layout.alignment: Qt.AlignTop
        }

        Rectangle {
            Layout.fillWidth: true
            height: 1
            color: g_systemPalette.dark
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
        Button {
            text: "Cycle Model Selection"
            onClicked: itemWindow.manager.select_next_model()
        }
        CheckBox {
            id: cameraView
            text: "Camera View"
            checked: false
        }
        CheckBox {
            id: showModelIndicator
            text: "Show Model Indicator"
            checked: false
        }
    }
}

