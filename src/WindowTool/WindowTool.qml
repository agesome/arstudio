import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Layouts 1.0

ColumnLayout {
    spacing: 5
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

