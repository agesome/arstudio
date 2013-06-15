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

    ItemWindow {
        id: itemwindow
        width: 500
        height: 500
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
}

