#include "stl_mesh.h"
#include "stl_mesh_geom.h"

#include <QNode>
#include <QUrl>


stl_mesh::stl_mesh(QNode *parent) : Qt3DRender::QGeometryRenderer(parent)
{
    stl_mesh_geom *geometry = new stl_mesh_geom(this);
    QGeometryRenderer::setGeometry(geometry);

    sl_load(QUrl("file:///home/varg/Downloads/cube.stl"));
}

void stl_mesh::sl_load(const QUrl &filename) {
    static_cast<stl_mesh_geom *>(geometry())->load(filename);
}
