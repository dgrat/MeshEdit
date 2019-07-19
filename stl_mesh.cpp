#include "stl_mesh.h"
#include "stl_mesh_geom.h"
#include "ffd/bezier_function.h"
#include "ctrl_points.h"

#include <array>
#include <QEntity>
#include <QNode>
#include <QUrl>
#include <QTransform>

#include <omp.h>


namespace {
    using arr3_1D = std::array<glm::vec3, 3>;
    using arr3_2D = std::array<arr3_1D, 3>;
    using arr3_3D = std::array<arr3_2D, 3>;

    glm::vec3 bezier333(const glm::vec3 &inp, const arr3_3D &ctrl_pts) {
        return bezier<3,3,3>(inp, ctrl_pts);
    }
};

stl_mesh::stl_mesh(QNode *parent) : Qt3DRender::QGeometryRenderer(parent)
{
    stl_mesh_geom *geometry = new stl_mesh_geom(this);
    QGeometryRenderer::setGeometry(geometry);

    sl_load(QUrl("file:///home/varg/Downloads/cube.stl"));
}

void stl_mesh::sl_load(const QUrl &filename) {
    static_cast<stl_mesh_geom *>(geometry())->load(filename);
}

void stl_mesh::sl_performFFD(const ctrl_points &ctrl_pt_entity)
{
    const auto &ctrl_pt_entities = ctrl_pt_entity.getCtrlPoints();
    // generate 3D ctrl pt array from currently active Qt3D entities
    arr3_3D ctrl_pts;
    #pragma omp parallel for
    for(size_t i = 0; i < ctrl_pt_entities.size(); i++) {
        auto &p = *ctrl_pt_entities.begin()++;
        auto &index = p.second;
        auto &entity = p.first;

        QVector3D pos;
        for (auto *component : entity->components()) {
            Qt3DCore::QTransform *trafo = dynamic_cast<Qt3DCore::QTransform *>(component);
            if(trafo) {
                pos = trafo->translation();
                break;
            }
        }

        const int x = index[0];
        const int y = index[1];
        const int z = index[2];
        ctrl_pts[x][y][z] = glm::vec3(pos.x(), pos.y(), pos.z());
    }

    // copy faces
    _ffd_mesh.clear();
    _ffd_mesh = static_cast<stl_mesh_geom *>(geometry())->faces();
    // go over the faces (could use OpenMP)
    #pragma omp parallel for
    for(size_t i = 0; i < _ffd_mesh.size(); i++) {
        auto &face = _ffd_mesh.at(i);
        face._vert_1 = bezier333(face._vert_1, ctrl_pts);
        face._vert_2 = bezier333(face._vert_2, ctrl_pts);
        face._vert_3 = bezier333(face._vert_3, ctrl_pts);
        face._norm = bezier333(face._norm, ctrl_pts);
    }

    static_cast<stl_mesh_geom *>(geometry())->refresh(_ffd_mesh);
}
