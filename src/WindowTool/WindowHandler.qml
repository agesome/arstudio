import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Layouts 1.0

import arstudio 1.0

/**
  This component is responsible for displaying a list of sequences
  available for display in an ItemWindow and for adding user-selected
  sequences to ItemWindow's Scenegraph.
*/

ColumnLayout {
    property bool exclusiveMode: false
    property int exclusiveTo: 0

    readonly property ItemWindow window: itemwindow
    property alias cameraView: itemwindow.cameraView
    property alias skyboxSource: itemwindow.skyboxSource

    property vector3d modelPosition
    property vector3d modelRotation

    onModelPositionChanged: itemwindow.selectedModel.position = modelPosition
    onModelRotationChanged: itemwindow.selectedModel.rotation = modelRotation

    ItemWindow {
        id: itemwindow
        width: 500
        height: 500
        onSelectedModelChanged: {
            modelPositionX.value = selectedModel.position.x
            modelPositionY.value = selectedModel.position.y
            modelPositionZ.value = selectedModel.position.z
            modelRotationX.value = selectedModel.rotation.x
            modelRotationY.value = selectedModel.rotation.y
            modelRotationZ.value = selectedModel.rotation.z
        }
    }

    TableView {

        Layout.fillHeight: true
        Layout.fillWidth: true
        model: g_Repository


        itemDelegate: CheckBox {
            anchors.fill: parent
            text: styleData.value
            enabled: !(exclusiveMode && (styleData.row !== exclusiveTo))

            MouseArea {
                anchors.fill: parent
                onClicked: checked = !checked
            }

            onCheckedChanged: {
                var sequence = model.get(styleData.row).ptr
                var scenegraph = itemwindow.manager.scenegraph
                if (checked)
                    scenegraph.add_sequence(sequence)
                else
                    scenegraph.remove_sequence(sequence)

                var type = model.get(styleData.row).type
                if (type === Sequence.BITMAP) {
                    if (checked) {
                        exclusiveMode = true
                        exclusiveTo = styleData.row
                    }
                    else
                        exclusiveMode = false
                }
            }
        }

        TableViewColumn {
            role: "name"
            title: "Repository"
            width: parent.width - 20
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

    GroupBox {
        title: "Model controls"
        flat: false
        enabled: itemwindow.manager.selected_model
        RowLayout {

            ColumnLayout {

                SpinBox {
                    Layout.minimumWidth: 95
                    Layout.fillHeight: true
                    id: modelPositionX
                    prefix: "X: "
                    stepSize: 0.01
                    decimals: 3
                    minimumValue: -10
                    maximumValue: 10
                    onValueChanged: modelPosition.x = value
                }
                SpinBox {
                    Layout.minimumWidth: 95
                    Layout.fillHeight: true
                    id: modelPositionY
                    prefix: "Y: "
                    stepSize: 0.01
                    decimals: 3
                    minimumValue: -10
                    maximumValue: 10
                    onValueChanged: modelPosition.y = value
                }
                SpinBox {
                    Layout.minimumWidth: 95
                    Layout.fillHeight: true
                    id: modelPositionZ
                    prefix: "Z: "
                    stepSize: 0.01
                    decimals: 3
                    minimumValue: -10
                    maximumValue: 10
                    onValueChanged: modelPosition.z = value
                }
            }
            ColumnLayout {
                SpinBox {
                    Layout.minimumWidth: 95
                    Layout.fillHeight: true
                    id: modelRotationX
                    prefix: "α: "
                    stepSize: 1
                    minimumValue: -180
                    maximumValue: 180
                    onValueChanged: modelRotation.x = value
                }
                SpinBox {
                    Layout.minimumWidth: 95
                    Layout.fillHeight: true
                    id: modelRotationY
                    prefix: "β: "
                    stepSize: 1
                    minimumValue: -180
                    maximumValue: 180
                    onValueChanged: modelRotation.y = value
                }
                SpinBox {
                    Layout.minimumWidth: 95
                    Layout.fillHeight: true
                    id: modelRotationZ
                    prefix: "γ: "
                    stepSize: 1
                    minimumValue: -180
                    maximumValue: 180
                    onValueChanged: modelRotation.z = value
                }
            }
        }
    }
}

