import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Layouts 1.0

import Qt3D 2.0
import Qt3D.Shapes 2.0

import arstudio 1.0
import WindowTool 1.0
import TimeLine 1.0

ApplicationWindow {
    id: g_rootWindow

    title: "CVAR Studio"
    width: 800
    height: 600

    menuBar: MainMenuBar {}
    toolBar: MainToolBar {}

    SystemPalette { id: g_systemPalette }

    ColumnLayout {
        anchors {
            fill: parent
            margins: 5
        }


        RowLayout {
            anchors {
                top: parent.top
                bottom: timeline.top
                left: parent.left
                right: parent.right
            }

            WindowTool {
                id: windowTool
                anchors {
                    left: parent.left
                    top: parent.top
                    bottom: parent.bottom
                }
            }

            Rectangle {
                id: g_displayArea
                anchors {
                    left: windowTool.right
                    right: parent.right
                    top: parent.top
                    bottom: parent.bottom
                }
                color: g_systemPalette.window
            }
        }

        TimeLine {
            id: timeline
            anchors {
                left: parent.left
                right: parent.right
                bottom: parent.bottom
            }
        }
    }
}
