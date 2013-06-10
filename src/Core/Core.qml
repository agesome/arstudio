import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Layouts 1.0
import QtQuick.Window 2.1
import QtQuick.Dialogs 1.0

import arstudio 1.0
import WindowTool 1.0
import TimeLine 1.0
import ProcessingDialog 1.0

ApplicationWindow {
    id: g_rootWindow

    title: "CVAR Studio"
    width: 800
    height: 600

    visibility: menubar.goFullscreen ? Window.FullScreen : Window.Windowed

    menuBar: MainMenuBar { id: menubar }
    toolBar: MainToolBar {}

    SystemPalette { id: g_systemPalette }

    ColumnLayout {
        anchors {
            fill: parent
            margins: 5
        }

        WindowTool { visible: menubar.showWindowTool }

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
        nameFilters: [ "Kinvideo files(*.kinvideo)" ]
        onAccepted: menubar.showOpenFile = false
        onRejected: menubar.showOpenFile = false
        visible: menubar.showOpenFile
    }

    ProcessingDialog { sourceFile: fd.fileUrl }
}
