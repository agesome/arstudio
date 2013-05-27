import QtQuick 2.0
import QtQuick.Controls 1.0

MenuBar {
    Menu {
        title: "File"

        MenuItem {
            text: "Open"
            shortcut: "Ctrl+O"
            iconName: "document-open"
        }

        MenuItem {
            text: "Record"
            shortcut: "Ctrl+R"
            iconName: "document-new"
        }

        MenuSeparator {}

        MenuItem {
            text: "Quit"
            shortcut: "Ctrl+Q"
            iconName: "application-exit"
            onTriggered: Qt.quit()
        }
    }
}
