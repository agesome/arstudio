import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Window 2.1

import Qt3D 2.0
import Qt3D.Shapes 2.0

import arstudio 1.0

/**
  This component implements a window for displaying items from Repository.
*/

Window {
    id: window

    property url skyboxSource
    property bool cameraView: false
    property bool showModelIndicator: false

    signal modelChanged(vector3d position, vector3d rotation, real scale)

    flags: Qt.Tool
    Component.onCompleted: show()

    onSkyboxSourceChanged: skybox.visible = true
    // have to contain the Viewport in a rectangle,
    // or navigation does not work
    Rectangle {
        anchors.fill: parent
        color: "#00000000"

        ItemView {
            anchors.fill: parent
        }

    }
}
