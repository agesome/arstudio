import QtQuick 2.1
import Qt3D 2.0
import Qt3D.Shapes 2.0

Item3D {
    Cube {
        scale: 0.1
        position: Qt.vector3d(1, 0, 0)
        effect: Effect {
            id: effect_x
            color: "#ff0000"
        }
    }

    Line {
        width: 2
        vertices: [1, 0, 0, -1, 0, 0]
        effect: effect_x
    }

    Cube {
        scale: 0.1
        position: Qt.vector3d(0, 1, 0)
        effect: Effect {
            id: effect_y
            color: "#00ff00"
        }
    }

    Line {
        width: 2
        vertices: [0, 1, 0, 0, -1, 0]
        effect: effect_y
    }

    Cube {
        scale: 0.1
        position: Qt.vector3d(0, 0, 1)
        effect: Effect {
            id: effect_z
            color: "#0000ff"
        }
    }

    Line {
        width: 2
        vertices: [0, 0, 1, 0, 0, -1]
        effect: effect_z
    }
}
