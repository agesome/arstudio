import QtQuick 2.0
import Qt3D 2.0
import Qt3D.Shapes 2.0

Item3D {
    Cube {
        scale: 0.1
        position: Qt.vector3d(1, 0, 0)
        effect: Effect {
            color: "#ff0000"
        }
    }

    Cube {
        scale: 0.1
        position: Qt.vector3d(-1, 0, 0)
        effect: Effect {
            color: "#ffffff"
        }
    }

    Cube {
        scale: 0.1
        position: Qt.vector3d(0, 1, 0)
        effect: Effect {
            color: "#00ff00"
        }
    }

    Cube {
        scale: 0.1
        position: Qt.vector3d(0, -1, 0)
        effect: Effect {
            color: "#ffffff"
        }
    }

    Cube {
        scale: 0.1
        position: Qt.vector3d(0, 0, 1)
        effect: Effect {
            color: "#0000ff"
        }
    }

    Cube {
        scale: 0.1
        position: Qt.vector3d(0, 0, -1)
        effect: Effect {
            color: "#ffffff"
        }
    }
}
