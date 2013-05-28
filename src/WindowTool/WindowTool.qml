import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Layouts 1.0

ColumnLayout {
    id: column
    spacing: 5

    Text {
        id: wl
        anchors.top: parent.top
        text: "Window List"
        renderType: Text.NativeRendering
    }

    TabView {
        id: tabview

        anchors.left: parent.left
        anchors.top: wl.bottom
        anchors.bottom: buttonRow.top
    }

    RowLayout {
        id: buttonRow

        anchors.bottom: column.bottom

        Button {
            text: "Add window"
            onClicked: tabview.addTab(tabview.count + 1, windowHandler)

            Component {
                id: windowHandler
                WindowHandler {}
            }
        }
    }
}

