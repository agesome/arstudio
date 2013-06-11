import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Layouts 1.0

/**
  This component is responsible for creating and managing
  instances of ItemWindow handlers (WindowHandler).
*/

ColumnLayout {
    Layout.fillWidth: false

    TabView {
        id: tabview
        Layout.fillHeight: true
        Layout.fillWidth: true
    }
    Button {
        Layout.fillWidth: true
        text: "Add window"
        onClicked: tabview.addTab(tabview.count + 1, windowHandler)

        Component {
            id: windowHandler
            WindowHandler {}
        }
    }
}

