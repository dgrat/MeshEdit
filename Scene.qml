import Qt3D.Core 2.0
import Qt3D.Render 2.0
import Qt3D.Input 2.0
import Qt3D.Extras 2.0

import QtQuick 2.0

import own.classes.ctrl_points 1.0


Entity {
    id: sceneRoot

    Camera {
        id: camera
        projectionType: CameraLens.PerspectiveProjection
        fieldOfView: 45
        nearPlane : 0.1
        farPlane : 1000.0
        position: Qt.vector3d( 0.0, 0.0, 5.0 )
        upVector: Qt.vector3d( 0.0, 1.0, 0.0 )
        viewCenter: Qt.vector3d( 0.0, 0.0, 0.0 )
    }

    FirstPersonCameraController { camera: camera }

    components: [
        RenderSettings {
            activeFrameGraph: ForwardRenderer {
                camera: camera
                clearColor: "transparent"
            }
        },
        InputSettings { }
    ]

    PhongMaterial {
        id: material
    }

    CtrlPtEntity {
        id: ctrlPoints
        radius: 0.05

        onEntityPressed: {
            console.log(ctrl_point, entity_position.x, entity_position.y, entity_position.z)

            coord_inp.set_ctrl_pt_position(entity_position.x, entity_position.y, entity_position.z)
            coord_inp.set_dial_position(mouse_position.x, mouse_position.y)
            coord_inp.set_entities(ctrlPoints, ctrl_point)
        }
    }
}