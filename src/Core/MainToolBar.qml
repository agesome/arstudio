import QtQuick 2.0
import QtQuick.Controls 1.0
import QtQuick.Layouts 1.0

ToolBar {
    RowLayout {
        ToolButton {
            text: "Clear Repository"
            iconName: "application-exit"
        }
        ToolButton {
            text: "Process"
            iconName: "system-run"
            onClicked: g_Pipeline.process()
        }
    }
}
