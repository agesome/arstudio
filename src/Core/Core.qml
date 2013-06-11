import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Layouts 1.0
import QtQuick.Window 2.1
import QtQuick.Dialogs 1.0

import arstudio 1.0
import WindowTool 1.0
import TimeLine 1.0
import ProcessingDialog 1.0
import ConfigView 1.0

/**
  This is the Core component, it is responsible for creating and managing
  all other components, provides menu and toolbar.
*/


ApplicationWindow {
    id: g_rootWindow

    title: "CVAR Studio"
    width: 500
    height: 450

    visibility: menubar.goFullscreen ? Window.FullScreen : Window.Windowed

    menuBar: MainMenuBar { id: menubar }
    toolBar: MainToolBar {}

    SystemPalette { id: g_systemPalette }

    ColumnLayout {
        anchors {
            fill: parent
            margins: 5
        }

        RowLayout {
            WindowTool { visible: menubar.showWindowTool }
            ConfigView {}
        }

        TimeLine {
            visible: menubar.showTimeline
            Layout.alignment: Qt.AlignBottom
        }
    }

    FileDialog {
        id: fd
        title: "Please choose a file"
        selectMultiple: false
        selectExisting: true
        selectFolder: false
        nameFilters: [ "Kinvideo files(*.kinvideo)", "All files(*.*)" ]
        onAccepted: menubar.showOpenFile = false
        onRejected: menubar.showOpenFile = false
        visible: menubar.showOpenFile
    }

    ProcessingDialog {
        id: processing_dialog
        sourceFile: fd.fileUrl
    }
}
