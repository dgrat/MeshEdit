#include "grid3x3.h"

CtrlPtEntity::CtrlPtEntity( Qt3DCore::QEntity* parent) : Qt3DCore::QEntity(parent)
{
        Qt3DRender::QMaterial *material = new Qt3DExtras::QPhongMaterial;
        Qt3DExtras::QSphereMesh *ctrl_pt = new Qt3DExtras::QSphereMesh;
        ctrl_pt->setRadius(_radius);

        Qt3DRender::QPickingSettings *settings = new Qt3DRender::QPickingSettings(this);
        settings->setFaceOrientationPickingMode(Qt3DRender::QPickingSettings::FrontFace);
        settings->setPickMethod(Qt3DRender::QPickingSettings::TrianglePicking);
        settings->setPickResultMode(Qt3DRender::QPickingSettings::NearestPick);

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
            QObject::connect(picker, &Qt3DRender::QObjectPicker::clicked, this, [=](Qt3DRender::QPickEvent *event){pressedEntity(event, entity);} );

            entity->addComponent(picker);
            entity->addComponent(material);
            entity->addComponent(ctrl_pt);
            entity->addComponent(pos);

            _CtrlPtList[{x,y,z}] = entity;
        }

        qDebug() << "ctor test";
}

Qt3DCore::QEntity *CtrlPtEntity::getPos(int x, int y, int z) const {
   return _CtrlPtList.at({x,y,z});
}

CtrlPtEntity::~CtrlPtEntity() {}

void CtrlPtEntity::pressedEntity(Qt3DRender::QPickEvent *event, Qt3DCore::QEntity *entity) {
    qDebug() << "test" << entity;

    for (auto *component : entity->components()) {
        Qt3DCore::QTransform *trafo = dynamic_cast<Qt3DCore::QTransform *>(component);
        if(trafo) {
            qDebug() << "trafo found";
            auto transl = trafo->translation();
            qDebug() << transl.x() << transl.y() << transl.z();
        }
    }
}
