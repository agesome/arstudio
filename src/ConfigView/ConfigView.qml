import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Layouts 1.0
import QtQuick.Controls.Styles 1.1

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
            width: tableview.width * 0.39
        }
    }

    RowLayout {
        Layout.fillWidth: true

        ComboBox {
            id: algoChecker
            model: g_Config.list
            Layout.fillWidth: true
            onCurrentTextChanged: g_Config.set_algo(currentText)
        }
        Button {
            id: defaultButton
            text: "Load default settings"
            onClicked: g_Config.load_defaults()
        }
    }

    Component {
        id: rectfield

        RowLayout {
            id: delegateLayout
            TextField {

                id: field
                text: styleData.value
                visible: true

                Layout.maximumHeight: delegateLayout.height
                Layout.fillWidth: true

                Keys.onPressed: {
                    if(event.key === Qt.Key_Return || event.key === Qt.Key_Enter)
                    {
                        g_Config.set(styleData.row, field.text)
                        textColor = "green"
                    }
                    else if (event.key >= Qt.Key_0 && event.key <= Qt.Key_Z)
                    {
                        textColor = "red"
                    }
                }
            }

            Button {
                id: button
                text: "S"
                Layout.maximumHeight: delegateLayout.height
                Layout.maximumWidth: delegateLayout.height

                onClicked: {
                    g_Config.set(styleData.row, field.text)
                    parent.textChanged = false
                }
            }
        }
    }
}

