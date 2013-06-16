import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Layouts 1.0
import QtQuick.Dialogs 1.0

/**
  This component is responsible for creating and managing
  instances of ItemWindow handlers (WindowHandler).
*/

ColumnLayout {
    property alias selectSkybox: skybox_selector.visible
    property alias selectModel: model_selector.visible
    property WindowHandler currentHandler: null

    Layout.fillWidth: false

    TabView {
        id: tabview
        Layout.fillHeight: true
        Layout.fillWidth: true
        onCurrentIndexChanged: currentHandler = getTab(currentIndex).item
    }

    Button {
        Layout.fillWidth: true
        text: "Add window"
        onClicked: {
            var tab = tabview.addTab(tabview.count + 1, windowHandler)
            if (currentHandler == null)
                currentHandler = tab.item
        }

        Component {
            id: windowHandler
            WindowHandler {}
        }
    }

    FileDialog {
        id: skybox_selector
        title: "Please choose a folder"
        selectMultiple: false
        selectExisting: true
        selectFolder: true
        onAccepted: currentHandler.skyboxSource = fileUrl
        visible: false
    }

    FileDialog {
        id: model_selector
        title: "Please choose a file"
        selectMultiple: false
        selectExisting: true
        selectFolder: false
        onAccepted: currentHandler.window.manager.add_custom_model(fileUrl)
        visible: false
    }
}

