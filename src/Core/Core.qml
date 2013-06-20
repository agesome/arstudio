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
    minimumWidth: 500
    minimumHeight: 500

    menuBar: MainMenuBar {
        id: menubar

        onShowOpenFile: openFileDialog.visible = true
    }

    toolBar: MainToolBar {}

    SystemPalette { id: g_systemPalette }

    ColumnLayout {
        anchors {
            fill: parent
            margins: 5
        }

        TabView {
            Layout.fillHeight: true
            Layout.fillWidth: true

            Tab {
                title: "Window Management"
                WindowTool {}
            }
            Tab {
                title: "Algorithm Settings"
                ConfigView {}
            }
        }

        TimeLine {
            Layout.alignment: Qt.AlignBottom
        }
    }

    FileDialog {
        id: openFileDialog
        title: "Please choose a file"
        selectMultiple: false
        selectExisting: true
        selectFolder: false
        nameFilters: [ "Kinvideo files(*.kinvideo)", "All files(*.*)" ]
    }

    ProcessingDialog {
        id: processing_dialog
        sourceFile: openFileDialog.fileUrl
    }
}
