import QtQuick 2.1
import QtQuick.Controls 1.0

MenuBar {
    property alias showWindowTool: show_window_tool.checked
    property alias showTimeline: show_timeline.checked
    property alias goFullscreen: go_fullscreen.checked

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

    Menu {
        title: "View"

        MenuItem {
            id: show_window_tool
            text: "Show Window Tool"
            checkable: true
            checked: true
            shortcut: "Ctrl+Shift+W"
        }

        MenuItem {
            id: show_timeline
            text: "Show Timeline"
            checkable: true
            checked: true
            shortcut: "Ctrl+Shift+T"
        }

        MenuItem {
            id: go_fullscreen
            text: "Full Screen"
            checkable: true
            checked: false
            shortcut: "Ctrl+Shift+F11"
        }
    }
}
