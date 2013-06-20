import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Layouts 1.0

GroupBox {
    id: root
    title: "Model controls"

    property vector3d position
    property vector3d rotation

    function reset (p, r) {
        modelPositionX.value = p.x
        modelPositionY.value = p.y
        modelPositionZ.value = p.z

        modelRotationX.value = r.x
        modelRotationY.value = r.y
        modelRotationZ.value = r.z
    }

    RowLayout {
        ColumnLayout {
            SpinBox {
                Layout.minimumWidth: 95
                Layout.fillHeight: true
                id: modelPositionX
                prefix: "X: "
                stepSize: 0.01
                decimals: 3
                minimumValue: -10
                maximumValue: 10
                onValueChanged: root.position.x = value
            }
            SpinBox {
                Layout.minimumWidth: 95
                Layout.fillHeight: true
                id: modelPositionY
                prefix: "Y: "
                stepSize: 0.01
                decimals: 3
                minimumValue: -10
                maximumValue: 10
                onValueChanged: root.position.y = value
            }
            SpinBox {
                Layout.minimumWidth: 95
                Layout.fillHeight: true
                id: modelPositionZ
                prefix: "Z: "
                stepSize: 0.01
                decimals: 3
                minimumValue: -10
                maximumValue: 10
                onValueChanged: root.position.z = value
            }
        }
        ColumnLayout {
            SpinBox {
                Layout.minimumWidth: 95
                Layout.fillHeight: true
                id: modelRotationX
                prefix: "α: "
                stepSize: 1
                minimumValue: -180
                maximumValue: 180
                onValueChanged: root.rotation.x = value
            }
            SpinBox {
                Layout.minimumWidth: 95
                Layout.fillHeight: true
                id: modelRotationY
                prefix: "β: "
                stepSize: 1
                minimumValue: -180
                maximumValue: 180
                onValueChanged: root.rotation.y = value
            }
            SpinBox {
                Layout.minimumWidth: 95
                Layout.fillHeight: true
                id: modelRotationZ
                prefix: "γ: "
                stepSize: 1
                minimumValue: -180
                maximumValue: 180
                onValueChanged: root.rotation.z = value
            }
        }
    }
}
