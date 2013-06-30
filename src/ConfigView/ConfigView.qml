import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Layouts 1.0

/**
  This component is a simple viewer for Config, using it as the
  model for a TableView
*/

TableView {
    id: tableview
    model: g_Config

    TableViewColumn {
        title: "Path"
        role: "path"
        width: tableview.width * 0.6
    }
    TableViewColumn {
        title: "Value"
        role: "value"
        delegate: TextField {
            text: styleData.value
            onTextChanged: g_Config.set (styleData.row, text)
        }
        width: tableview.width * 0.35
    }
}
