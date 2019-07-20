#include "ctrl_points.h"

ctrl_points::ctrl_points( Qt3DCore::QEntity* parent) : Qt3DCore::QEntity(parent)
{
        Qt3DRender::QMaterial *material = new Qt3DExtras::QPhongMaterial;
        Qt3DExtras::QSphereMesh *ctrl_pt = new Qt3DExtras::QSphereMesh;
        ctrl_pt->setRadius(_radius);

        Qt3DRender::QPickingSettings *settings = new Qt3DRender::QPickingSettings(this);
        //settings->setFaceOrientationPickingMode(Qt3DRender::QPickingSettings::FrontFace);
        settings->setPickMethod(Qt3DRender::QPickingSettings::TrianglePicking);
        //settings->setPickResultMode(Qt3DRender::QPickingSettings::NearestPick);

        for(int x = 0; x < _pts_x; x++)
        for(int y = 0; y < _pts_y; y++)
        for(int z = 0; z < _pts_z; z++) {
            Qt3DCore::QTransform *pos = new Qt3DCore::QTransform;
            pos->setTranslation(
                { static_cast<float>(x) / (_pts_x-1),
                  static_cast<float>(y) / (_pts_y-1),
                  static_cast<float>(z) / (_pts_z-1)
                }
            );

            auto entity = new Qt3DCore::QEntity(this);

            Qt3DRender::QObjectPicker *picker = new Qt3DRender::QObjectPicker(entity);
            QObject::connect(picker, &Qt3DRender::QObjectPicker::clicked, this, [=](Qt3DRender::QPickEvent *event){sl_entityPressed(event, entity);} );

            entity->addComponent(picker);
            entity->addComponent(material);
            entity->addComponent(ctrl_pt);
            entity->addComponent(pos);

            _CtrlPtList[entity] = {x,y,z};
        }
}

ctrl_points::~ctrl_points() {}

void ctrl_points::sl_entityPressed(Qt3DRender::QPickEvent *event, Qt3DCore::QEntity *entity) {
    for (auto *component : entity->components()) {
        Qt3DCore::QTransform *trafo = dynamic_cast<Qt3DCore::QTransform *>(component);
        if(trafo) {
            auto transl = trafo->translation();
            emit entityPressed(entity, QVector3D(transl.x(), transl.y(), transl.z()), event->position());
        }
    }
}

void ctrl_points::sl_changePosition(Qt3DCore::QEntity *ctrl_point, QVector3D position) {
    for (auto *component : ctrl_point->components()) {
        Qt3DCore::QTransform *trafo = dynamic_cast<Qt3DCore::QTransform *>(component);
        if(trafo) {
            trafo->setTranslation(position);
            emit positionChanged();
        }
    }
}
