#pragma once

#include "stl_import.h"

#include <QGeometryRenderer>
#include <glm/glm.hpp>

class QNode;
class QUrl;

class stl_mesh : public Qt3DRender::QGeometryRenderer
{
    Q_OBJECT

private:
    std::vector<stl::face> _ffd_mesh;

public:
    stl_mesh(QNode *parent = nullptr);

public slots:
    void sl_load(const QUrl &filename);
    void sl_performFFD(const std::map<Qt3DCore::QEntity *, std::array<int, 3>> &, const std::vector<stl::face> &);
};
