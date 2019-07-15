#include "grid3x3.h"

CtrlPtEntity::CtrlPtEntity( Qt3DCore::QEntity* parent) : Qt3DCore::QEntity(parent)
{
        Qt3DRender::QMaterial *material = new Qt3DExtras::QPhongMaterial;
        Qt3DExtras::QSphereMesh *ctrl_pt = new Qt3DExtras::QSphereMesh;
        ctrl_pt->setRadius(_radius);

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
            entity->addComponent(material);
            entity->addComponent(ctrl_pt);
            entity->addComponent(pos);

            _CtrlPtList[{x,y,z}] = entity;
        }
}

Qt3DCore::QEntity *CtrlPtEntity::getPos(int x, int y, int z) const {
   return _CtrlPtList.at({x,y,z});
}

CtrlPtEntity::~CtrlPtEntity() {}
