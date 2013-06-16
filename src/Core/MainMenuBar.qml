import QtQuick 2.1
import QtQuick.Controls 1.0

MenuBar {
    property alias showWindowTool: show_window_tool.checked
    property alias showTimeline: show_timeline.checked
    property alias goFullscreen: go_fullscreen.checked
    property alias cameraView: camera_view.checked

    property bool showOpenFile: false
    property bool haveCurrentWindow: false

    signal loadSkybox ()
    signal loadModel ()

    Menu {
        title: "File"

        MenuItem {
            text: "Open"
            shortcut: "Ctrl+O"
            iconName: "document-open"
            onTriggered: showOpenFile = true
        }

        MenuItem {
            text: "Record"
            shortcut: "Ctrl+R"
            iconName: "document-new"
        }

        MenuItem {
            text: "Load Skybox"
            shortcut: "Ctrl+Shift+S"
            iconName: "folder"
            enabled: haveCurrentWindow
            onTriggered: loadSkybox()
        }

        MenuItem {
            text: "Load custom model"
            shortcut: "Ctrl+Shift+M"
            iconName: "package-x-generic"
            enabled: haveCurrentWindow
            onTriggered: loadModel()
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

        MenuSeparator {}

        MenuItem {
            id: camera_view
            text: "Enable camera view"
            checkable: true
            checked: cameraView
            enabled: haveCurrentWindow
            shortcut: "Ctrl+Shift+C"
        }
    }
}
