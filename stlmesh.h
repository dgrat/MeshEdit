#pragma once

#include <QGeometryRenderer>
#include <QBuffer>

#include "stl_import.h"


class STLMesh : public Qt3DRender::QGeometryRenderer
{
    Qt3DRender::QGeometry *_geometry;
    Qt3DRender::QBuffer *_vertexBuffer;
    Qt3DRender::QBuffer *_indexBuffer;

    std::vector<stl::face> _faces_scaled;
    std::vector<stl::face> _faces_transformed;

public:
    STLMesh();

    void load(const QString &filename);
    void create_mesh();
};
