import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Layouts 1.0

/**
  This component is a simple viewer for Config, using it as the
  model for a TableView
*/



ColumnLayout {

    TableView {
        id: tableview
        model: g_Config
        Layout.fillWidth: true
        Layout.fillHeight: true

        TableViewColumn {
            title: "Path"
            role: "path"
            width: tableview.width * 0.6
        }

        TableViewColumn {
            title: "Value"
            role: "value"
            delegate: rectfield
            width: tableview.width * 0.35
        }
    }

    RowLayout{
        ComboBox {
            id: algoChecker
            width: tableview.width - defaultButton.width
            model: g_Config.list
            anchors.right: defaultButton.left
            onCurrentTextChanged: g_Config.set_algo(currentText)
            Button {
                id: defaultButton
                anchors.left: algoChecker.right
                anchors.verticalCenter: algoChecker.verticalCenter
                text: "Load default settings"
                onClicked: g_Config.load_defaults()
            }
        }
    }

    Component {
        id: rectfield
        Rectangle {
            property bool textChanged: false

            id: rect
            color: textChanged ? "yellow" : "white"

            TextField {
                id: field
                width: rect.width
                height: rect.height
                text: styleData.value
                visible: true
                Keys.onPressed: {
                    if(event.key === Qt.Key_Return || event.key === Qt.Key_Enter)
                    {
                        g_Config.set(styleData.row, field.text)
                        rect.textChanged = false
                    }
                }
            }

            Button {
                id: button
                anchors.left: field.right
                height: field.height
                width: 25
                text: "S"
                onClicked: {
                    g_Config.set(styleData.row, field.text)
                    parent.textChanged = false
                }

            }
        }
    }
}

