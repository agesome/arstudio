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
    minimumWidth: 500
    minimumHeight: 500

    menuBar: MainMenuBar {
        id: menubar

        onShowOpenFile: openFileDialog.visible = true
    }

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
