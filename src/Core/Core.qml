import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Layouts 1.0

import Qt3D 2.0
import Qt3D.Shapes 2.0

import arstudio 1.0
import WindowTool 1.0
import TimeLine 1.0

ApplicationWindow {
    id: g_rootWindow

    title: "CVAR Studio"
    width: 800
    height: 600

    menuBar: MainMenuBar {}
    toolBar: MainToolBar {}

    SystemPalette { id: g_systemPalette }

    ColumnLayout {
        anchors.fill: parent

        RowLayout {
            anchors.top: parent.top
            anchors.bottom: timeline.top
            anchors.left: parent.left
            anchors.right: parent.right

            WindowTool {
                id: windowTool
                anchors.left: parent.left
                anchors.top: parent.top
                anchors.bottom: parent.bottom
            }

            Rectangle {
                id: g_displayArea
                anchors.left: windowTool.right
                anchors.right: parent.right
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                color: g_systemPalette.window
            }
        }

        TimeLine {
            id: timeline
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
        }
    }
}

//Rectangle {
//    id: root
//    width: view.width
//    height: view.height
//    color: palette.window

//

//    function removeItems () {
//        console.log ("removing "
//                     + (vp.children.length - 1)
//                     + " current objects")
//        for (var i = 0; i < vp.children.length; i++)
//            if (vp.children[i] !== vp_axis)
//                vp.children[i].destroy ()
//        vp.update ()
//    }

//    function showItems (frame) {
//        console.log ("there are " + scenegraph.sequences.length + " sequences")
//        for (var i = 0; i < scenegraph.sequences.length; i++) {
//            var c = scenegraph.sequences[i]
//            console.log (c + " is of type " + c.type)

//            if (c.type === Sequence.CAMERA) {
//                if (frame !== "-1") {
//                    console.log ("showing a camera for frame " + frame)
//                    var item = c.item_for_frame (frame)

//                    cameraFactory.createObject (vp,
//                                                {
//                                                    position: item.position,
//                                                    rotation: item.rotation
//                                                })
//                    return
//                }

//                var mf = scenegraph.max_frame ()
//                console.log ("showing a camera for " + mf + " frames")
//                for (var f = 1; f < mf; f++) {
//                    var i = c.item_for_frame (f)
//                    console.log ("position: " + i.position)
//                    cameraFactory.createObject (vp,
//                                                {
//                                                    position: i.position,
//                                                    rotation: i.rotation
//                                                })
//                }
//            }
//        }
//    }

//    Component {
//        id: pointFactory
//        Cube {
//            scale: 0.1
//             Effect {
//                color: "#ff0000"
//            }
//        }
//    }

//    Component {
//        id: cameraFactory
//        Item3D {
//            property vector3d rotation: Qt.vector3d(0, 0, 0)
//            scale: 0.24
//            mesh: Mesh { source: "camera.obj" }
//            effect: Effect { color: Qt.rgba(Math.abs(rotation.x) * 250,
//                                            Math.abs(rotation.y) * 250,
//                                            0)
//            }
//            transform: [
//                Rotation3D {
//                    angle: rotation.x
//                    axis: Qt.vector3d(1, 0, 0)
//                },
//                Rotation3D {
//                    angle: rotation.y
//                    axis: Qt.vector3d(0, 1, 0)
//                },
//                Rotation3D {
//                    angle: rotation.z
//                    axis: Qt.vector3d(0, 0, 1)
//                }
//            ]
//        }
//    }

//    Subwindow {
//        x: 10
//        y: 300
//        width: 100
//        height: 100
//        color: palette.button
//        name: "start"

//        contents: Item {
//            TextInput {
//                id: frame_text
//                width: parent.width - parent.margin * 2
//                height: parent.height / 2
//                text: "-1"
//                cursorVisible: true
//                font.pointSize: 20
//                focus: true
//            }

//            MouseArea {
//                width: parent.width
//                height: parent.height

//                acceptedButtons: Qt.LeftButton | Qt.RightButton
//                onClicked: {
//                    if (mouse.button === Qt.LeftButton)
//                        root.showItems(frame_text.text)
//                    else
//                        root.removeItems()
//                }
//            }
//        }
//    }

//    Subwindow {
//        x: 120
//        y: 10
//        width: 500
//        height: 500
//        color: palette.dark
//        name: "view"

//        contents: Viewport {
//            id: vp
//            light: Light { ambientColor: "#ffffff" }
//            Axis { id: vp_axis }

//            Cube {
//                id: cube
//                scale: 1
//                position: Qt.vector3d(1, 0, 0)
//                effect: Effect {
//                    color: "#ff0000"
//                }
//                    CheckBox {
//                        x: cube.x
//                        y: cube.y
//                        text: qsTr("Breakfast")
//                    }
//            }
//        }
//    }

//    Subwindow {
//        x: 10
//        y: 100
//        fit_contents: true
//        color: palette.button
//        name: "process"

//        contents: Item {
//            width: dummy_text.width
//            height: dummy_text.height
//            Text {
//                id: progress_value
//                text: "00"
//                font.pointSize: 72
//                font.family: "Inconsolata"
//            }

//            Text {
//                id: dummy_text
//                text: "00"
//                font.pointSize: 72
//                font.family: "Inconsolata"
//                visible: false
//            }

//            Component.onCompleted: {
//                mouse.clicked.connect (pipeline.process)
//                pipeline.progress.connect (progress)
//            }

//            function progress (value, max) {
//                progress_value.text = value
//            }

//            MouseArea {
//                id: mouse
//                width: parent.width
//                height: parent.height
//            }
//        }
//    }

//    Subwindow {
//        x: 10
//        y: 520
//        fit_contents: true
//        name: "slider"
//        contents: Item {
//            id: slider

//            height: 30
//            width: 500

//            Rectangle {
//                height: 20
//                width: 10
//                x: 0
//                y: 5
//                color: "black"

//                MouseArea {
//                    anchors.fill: parent
//                    drag.target: parent
//                    drag.axis: Drag.XAxis
//                    drag.minimumX: 0
//                    drag.maximumX: slider.width - parent.width
//                }
//            }

//            Canvas {
//                id: canvas
//                height: parent.height
//                width: parent.width
//                onPaint: {
//                    var ctx = canvas.getContext('2d');
//                    ctx.moveTo (0, height/2)
//                    ctx.lineTo (width, height/2)
//                    ctx.stroke ()
//                }
//            }
//        }
//    }
//}
