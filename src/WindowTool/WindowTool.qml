import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Layouts 1.0
import QtQuick.Dialogs 1.0

/**
  This component is responsible for creating and managing
  instances of ItemWindow handlers (WindowHandler).
*/

ColumnLayout {
    property WindowHandler currentHandler: null

    TabView {
        id: windowList
        tabPosition: Qt.BottomEdge
        Layout.fillHeight: true
        Layout.fillWidth: true
        onCurrentIndexChanged: currentHandler = getTab(currentIndex).item
        visible: count != 0
    }

    Label {
        visible: windowList.count == 0
        text: "Click the '+' button to add a window"
        Layout.alignment: Qt.AlignCenter
    }

    RowLayout {
        Layout.alignment: Qt.AlignBottom
        Button {
            Layout.minimumWidth: height
            tooltip: "Add a Window"
            iconName: "list-add"
            onClicked: {
                var title = windowName.text
                var tab = windowList.addTab(windowName.text,
                                            windowHandler)
                tab.active = true
                tab.item.title = title
                if (currentHandler == null)
                    currentHandler = tab.item
            }
            Component {
                id: windowHandler
                WindowHandler {}
            }
        }
        TextField {
            id: windowName
            text: "Window #" + windowList.count
        }
    }

    FileDialog {
        id: skyboxSelector
        title: "Please choose a folder"
        selectMultiple: false
        selectExisting: true
        selectFolder: true
        onAccepted: currentHandler.skyboxSource = fileUrl
        visible: false
    }

    FileDialog {
        id: modelSelector
        title: "Please choose a file"
        selectMultiple: false
        selectExisting: true
        selectFolder: false
        onAccepted: currentHandler.windowManager.add_custom_model(fileUrl)
        nameFilters: [ "3D Models(*.obj *.3ds)" ]
        visible: false
    }
}

