import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Layouts 1.0

ColumnLayout {
    id: root

    property vector3d position
    property vector3d rotation
    property real scale

    function reset (p, r, s) {
        modelPositionX.value = p.x
        modelPositionY.value = p.y
        modelPositionZ.value = p.z

        modelRotationX.value = r.x
        modelRotationY.value = r.y
        modelRotationZ.value = r.z

        modelScale.value = s
    }
    Label {
        text: "<b>Model Controls</b>"
    }

    RowLayout {
        ColumnLayout {
            SpinBox {
                Layout.fillWidth: true
                id: modelPositionX
                prefix: "X: "
                stepSize: 0.01
                decimals: 2
                minimumValue: -10
                maximumValue: 10
                onValueChanged: root.position.x = value
            }
            SpinBox {
                Layout.fillWidth: true
                id: modelPositionY
                prefix: "Y: "
                stepSize: 0.01
                decimals: 2
                minimumValue: -10
                maximumValue: 10
                onValueChanged: root.position.y = value
            }
            SpinBox {
                Layout.fillWidth: true
                id: modelPositionZ
                prefix: "Z: "
                stepSize: 0.01
                decimals: 2
                minimumValue: -10
                maximumValue: 10
                onValueChanged: root.position.z = value
            }
        }
        ColumnLayout {
            SpinBox {
                Layout.fillWidth: true
                id: modelRotationX
                prefix: "α: "
                stepSize: 1
                minimumValue: -180
                maximumValue: 180
                onValueChanged: root.rotation.x = value
            }
            SpinBox {
                Layout.fillWidth: true
                id: modelRotationY
                prefix: "β: "
                stepSize: 1
                minimumValue: -180
                maximumValue: 180
                onValueChanged: root.rotation.y = value
            }
            SpinBox {
                Layout.fillWidth: true
                id: modelRotationZ
                prefix: "γ: "
                stepSize: 1
                minimumValue: -180
                maximumValue: 180
                onValueChanged: root.rotation.z = value
            }
        }
    }
    RowLayout {
        Label {
            text: "Scale: "
        }
        SpinBox {
            Layout.fillWidth: true
            Layout.maximumWidth: 100
            id: modelScale
            stepSize: 0.001
            decimals: 3
            minimumValue: 0
            maximumValue: 1
            onValueChanged: root.scale = value
        }
    }
}
