#pragma once

#include <array>
#include <map>

#include <Qt3DCore>
#include <Qt3DRender>
#include <Qt3DInput>
#include <Qt3DLogic>
#include <Qt3DExtras>
#include <Qt3DAnimation>


class CtrlPtEntity : public Qt3DCore::QEntity {
    Q_OBJECT
    Q_PROPERTY(int x_points READ xPoints WRITE setXPoints NOTIFY xPointsChanged)
    Q_PROPERTY(int y_points READ yPoints WRITE setYPoints NOTIFY yPointsChanged)
    Q_PROPERTY(int z_points READ zPoints WRITE setZPoints NOTIFY zPointsChanged)
    Q_PROPERTY(float radius READ radius WRITE setRadius NOTIFY radiusChanged)

    float _radius = 0.05f;
    int _pts_x = 3;
    int _pts_y = 3;
    int _pts_z = 3;

    std::map<std::array<int, 3>, Qt3DCore::QEntity *> _CtrlPtList;

public:
    CtrlPtEntity( Qt3DCore::QEntity* parent = nullptr);
    virtual ~CtrlPtEntity();

    float radius() const { return _radius; }
    int xPoints() const { return _pts_x; }
    int yPoints() const { return _pts_y; }
    int zPoints() const { return _pts_z; }

    void setRadius(float val) { if(val > 0) _radius = val; emit radiusChanged(); }
    void setXPoints(int x) { _pts_x = x; emit xPointsChanged(); }
    void setYPoints(int y) { _pts_y = y; emit yPointsChanged(); }
    void setZPoints(int z) { _pts_z = z; emit zPointsChanged(); }

    Qt3DCore::QEntity *getPos(int x, int y, int z) const;

signals:
    void radiusChanged();
    void xPointsChanged();
    void yPointsChanged();
    void zPointsChanged();
};

