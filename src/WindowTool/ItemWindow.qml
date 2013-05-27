import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Window 2.1

import Qt3D 2.0
import Qt3D.Shapes 2.0

import arstudio 1.0

Window {
    title: "Item Window"
    flags: Qt.Tool | Qt.SubWindow

    property int current_frame: 1
    property Scenegraph scenegraph: null

    Component.onCompleted: show()

    // have to contain the Viewport in a rectangle, or navigation does not work
    Rectangle {
        anchors.fill: parent
        color: g_systemPalette.dark

        Viewport {
            id: viewport
            anchors.fill: parent

            light: Light { ambientColor: "#ffffff" }
            Axis { id: vp_axis }
        }
    }

    function update(frame) {
        if (frame === -1)
            frame = current_frame

        var len = scenegraph.sequences.length
        for (var i = 0; i < len; i++) {
            var seq = scenegraph.sequences[i]
            var item = seq.item_for_frame(frame)

            if (item === null)
                continue;
            switch (seq.type) {
            case Sequence.CAMERA:
                cameraFactory.createObject(viewport,
                                           {
                                               position: item.position,
                                               rotation: item.rotation
                                           })
                break;
            }

        }
    }

    Component {
        id: pointFactory
        Cube {
            scale: 0.1
            Effect {
                color: "#ff0000"
            }
        }
    }

    Component {
        id: cameraFactory
        Cylinder {
            property vector3d rotation: Qt.vector3d(0, 0, 0)
            length: 0.05
            radius: 0.01
            effect: Effect { color: Qt.rgba(Math.abs(rotation.x) * 250,
                                            Math.abs(rotation.y) * 250,
                                            0)
            }
            transform: [
                Rotation3D {
                    angle: rotation.x
                    axis: Qt.vector3d(1, 0, 0)
                },
                Rotation3D {
                    angle: rotation.y
                    axis: Qt.vector3d(0, 1, 0)
                },
                Rotation3D {
                    angle: rotation.z
                    axis: Qt.vector3d(0, 0, 1)
                }
            ]
        }
    }
}
