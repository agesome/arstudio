import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Window 2.1

import Qt3D 2.0
import Qt3D.Shapes 2.0

import arstudio 1.0

Window {
    property IWManager manager: manager

    title: "Item Window"
    flags: Qt.Tool | Qt.SubWindow
    Component.onCompleted: show()

    // have to contain the Viewport in a rectangle,
    // or navigation does not work
    Rectangle {
        anchors.fill: parent
        color: "black"

        Viewport {
            id: viewport
            anchors.fill: parent
            light: Light { ambientColor: "#ffffff" }
            Axis { objectName: "vp_axis" }

        }
    }

    IWManager {
        id: manager
        viewport: viewport
    }
}
