#pragma once

#include "stl_import.h"

#include <QGeometryRenderer>
#include <QVector3D>
#include <glm/glm.hpp>

class QNode;
class QUrl;
class ctrl_points;

class stl_mesh : public Qt3DRender::QGeometryRenderer
{
    Q_OBJECT
    Q_PROPERTY(QVector3D dim MEMBER _dim NOTIFY dimChanged)

private:
    std::vector<stl::face> _ffd_mesh;
    QVector3D _dim;

public:
    stl_mesh(QNode *parent = nullptr);

public slots:
    void sl_load(const QUrl &filename);
    void sl_performFFD(ctrl_points *);

signals:
    void dimChanged();
};
