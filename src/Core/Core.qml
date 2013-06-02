import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Layouts 1.0
import QtQuick.Window 2.1

import arstudio 1.0
import WindowTool 1.0
import TimeLine 1.0

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
        spacing: 5

        RowLayout {
            WindowTool { visible: menubar.showWindowTool }

            Rectangle {
                Layout.minimumHeight: 1
                Layout.minimumWidth: 1
                Layout.fillHeight: true
                Layout.fillWidth: true
                color: g_systemPalette.dark
            }
        }

        TimeLine {
            id: g_TimeLine
            visible: menubar.showTimeline
        }
    }
}
