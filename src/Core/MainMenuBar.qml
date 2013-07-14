import QtQuick 2.1
import QtQuick.Controls 1.0

MenuBar {
    signal showOpenFile()
    signal openProcessing()

    property alias compactMode: compactMode.checked

    Menu {
        title: "File"

        MenuItem {
            text: "Open"
            shortcut: "Ctrl+O"
            iconName: "document-open"
            onTriggered: openProcessing()
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

        MenuItem {
            text: "Dump Repository As Text"
            onTriggered: g_Repository.dump_contents("test.asr")
        }

        MenuItem {
            text: "restore"
            onTriggered: g_Repository.populate_from_file ("test.asr");
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
