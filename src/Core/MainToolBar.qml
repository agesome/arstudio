import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Layouts 1.0

ToolBar {
    RowLayout {
        ToolButton {
            text: "Clear Repository"
            onClicked: g_Repository.clear()
        }
        ToolButton {
            text: "Processing dialog"
            onClicked: processing_dialog.show()
            enabled: processing_dialog.video_helper.status
        }
    }
}
