import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Layouts 1.0
import QtQuick.Window 2.1

import arstudio 1.0
import WindowTool 1.0
import TimeLine 1.0
import ProcessingTool 1.0
import ConfigView 1.0

/**
  This is the Core component, it is responsible for creating and managing
  all other components, provides menu and toolbar.
*/

ApplicationWindow {
    id: g_rootWindow

    title: "CVAR Studio"

    width: layout.implicitWidth
    height: layout.implicitHeight
    minimumWidth: layout.Layout.minimumWidth
    // FIXME: a way to determine menubar height?
    minimumHeight: layout.Layout.minimumHeight + 35

    menuBar: MainMenuBar {
        id: menubar

        onShowOpenFile: openFileDialog.visible = true
    }

    SystemPalette { id: g_systemPalette }

    ColumnLayout {
        id: layout
        anchors {
            fill: parent
            margins: 5
        }
        Layout.minimumWidth: 500

        TabView {
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.minimumHeight: 400
            visible: !menubar.compactMode

            Tab {
                title: "Window Management"
                WindowTool {}
            }
            Tab {
                title: "Algorithm Settings"
                ConfigView {}
            }
            Tab {
                title: "Processing"
                ProcessingTool {}
            }
        }

        TimeLine {
            Layout.alignment: Qt.AlignBottom
        }
    }
}
