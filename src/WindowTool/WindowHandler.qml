import QtQuick 2.1
import QtQuick.Controls 1.0

import arstudio 1.0

Item {
    property bool exclusiveMode: false
    property int exclusiveTo: 0

    TableView {
        anchors.fill: parent
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

    ItemWindow {
        id: itemwindow
        width: 400
        height: 400
    }
}

