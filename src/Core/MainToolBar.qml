import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Layouts 1.0

ToolBar {
    RowLayout {
        ToolButton {
            text: "Clear Repository"
            iconName: "application-exit"
            onClicked: g_Repository.clear()
        }
    }
}
