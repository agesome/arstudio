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
    visible: true

    property alias view: itemView

    ItemView {
        id: itemView
        anchors.fill: parent
    }
}
