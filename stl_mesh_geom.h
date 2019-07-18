#pragma once

#include <Qt3DRender/QGeometry>
#include <vector>

namespace stl {
    struct face;
};
class QNode;
namespace Qt3DRender {
    class QBuffer;
};

class stl_mesh_geom : public Qt3DRender::QGeometry
{
    Q_OBJECT
    Q_PROPERTY(Qt3DRender::QAttribute *positionAttribute READ positionAttribute CONSTANT)
    Q_PROPERTY(Qt3DRender::QAttribute *normalAttribute READ normalAttribute CONSTANT)
    Q_PROPERTY(Qt3DRender::QAttribute *indexAttribute READ indexAttribute CONSTANT)

private:
    void init();

    std::vector<stl::face> _stl_data;

    Qt3DRender::QAttribute *m_positionAttribute;
    Qt3DRender::QAttribute *m_normalAttribute;
    Qt3DRender::QAttribute *m_indexAttribute;

    Qt3DRender::QBuffer *m_vertexBuffer;
    Qt3DRender::QBuffer *m_indexBuffer;

public:
    stl_mesh_geom(QNode *parent = nullptr);

    void load(const QUrl &filename);

    Qt3DRender::QAttribute *positionAttribute() const;
    Qt3DRender::QAttribute *normalAttribute() const;
    Qt3DRender::QAttribute *indexAttribute() const;
};

