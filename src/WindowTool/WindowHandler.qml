import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Layouts 1.0
import QtQuick.Window 2.1

import arstudio 1.0

/**
  This component is responsible for displaying a list of sequences
  available for display in an ItemWindow and for adding user-selected
  sequences to ItemWindow's Scenegraph.
*/

RowLayout {
    id: root
    property int exclusiveSequenceIndex

    property alias title: window.title
    property alias scenegraph: itemView.scenegraph

    Window {
        id: window
        visible: true

        ItemView {
            id: itemView
            anchors.fill: parent
        }
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
}

