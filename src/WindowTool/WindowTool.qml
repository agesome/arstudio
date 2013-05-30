import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Layouts 1.0

ColumnLayout {
    id: column
    spacing: 5

    TabView {
        id: tabview
        anchors {
            left: parent.left
            top: parent.top
            bottom: buttonRow.top
        }
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

