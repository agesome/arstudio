import QtQuick 2.1
import QtQuick.Controls 1.0

MenuBar {
    signal showOpenFile()
    property alias compactMode: compactMode.checked

    Menu {
        title: "File"

        MenuItem {
            text: "Open"
            shortcut: "Ctrl+O"
            iconName: "document-open"
            onTriggered: showOpenFile()
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

    Menu {
        title: "Tools"

        MenuItem {
            text: "Clear Repository"
            shortcut: "Ctrl+Shift+C"
            iconName: "edit-cut"
            onTriggered: g_Repository.clear()
        }
    }

    Menu {
        title: "View"

        MenuItem {
            id: compactMode
            text: "Compact Mode"
            checkable: true
        }
    }
}
