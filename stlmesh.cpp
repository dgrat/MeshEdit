#include "stlmesh.h"

#include <QFile>
#include <Qt3DCore>
#include <Qt3DInput>
#include <QBuffer>
#include <QAttribute>


STLMesh::STLMesh() {
    _geometry = new Qt3DRender::QGeometry(this);
    _vertexBuffer = new Qt3DRender::QBuffer(_geometry);
    _indexBuffer = new Qt3DRender::QBuffer(_geometry);
    this->setGeometry(_geometry);
}

void STLMesh::load(const QString &filename) {
    stl::format stl_loader(filename.toStdString());
    auto bbox = stl_loader.estimate_bbox(stl_loader.faces());
    _faces_scaled = stl_loader.normalized(bbox);
    _faces_transformed = _faces_scaled;
}

void STLMesh::create_mesh() {
    QByteArray index_buf;
    index_buf.resize(_faces_transformed.size() * 3 * sizeof(uint32_t));
    uint32_t *index_buf_acc = reinterpret_cast<uint32_t *>(index_buf.data());

    QByteArray mesh_buf;
    mesh_buf.resize(_faces_transformed.size() * 3 * sizeof(glm::vec3) );
    glm::vec3 *mesh_buf_acc = reinterpret_cast<glm::vec3 *>(mesh_buf.data());

    uint32_t vert_ind = 0;
    uint32_t face_ind = 0;
    for (auto &face : _faces_transformed) {
        mesh_buf_acc[vert_ind++] = face._vert_1;
        mesh_buf_acc[vert_ind++] = face._norm;
        mesh_buf_acc[vert_ind++] = glm::vec3(0.5f);

        mesh_buf_acc[vert_ind++] = face._vert_2;
        mesh_buf_acc[vert_ind++] = face._norm;
        mesh_buf_acc[vert_ind++] = glm::vec3(0.5f);

        mesh_buf_acc[vert_ind++] = face._vert_3;
        mesh_buf_acc[vert_ind++] = face._norm;
        mesh_buf_acc[vert_ind++] = glm::vec3(0.5f);

        index_buf_acc[face_ind+0] = face_ind+0;
        index_buf_acc[face_ind+1] = face_ind+1;
        index_buf_acc[face_ind+2] = face_ind+2;
        face_ind += 3;
    }

    _vertexBuffer->setData(mesh_buf);
    _indexBuffer->setData(index_buf);

    Qt3DRender::QAttribute *positionAttribute = new Qt3DRender::QAttribute();
    positionAttribute->setAttributeType(Qt3DRender::QAttribute::VertexAttribute);
    positionAttribute->setBuffer(_vertexBuffer);
    positionAttribute->setVertexBaseType(Qt3DRender::QAttribute::Float);
    positionAttribute->setByteOffset(0);
    positionAttribute->setByteStride(3 * sizeof(glm::vec3));
    positionAttribute->setCount(_faces_transformed.size());
    positionAttribute->setName(Qt3DRender::QAttribute::defaultPositionAttributeName());

    Qt3DRender::QAttribute *normalAttribute = new Qt3DRender::QAttribute();
    normalAttribute->setAttributeType(Qt3DRender::QAttribute::VertexAttribute);
    normalAttribute->setBuffer(_vertexBuffer);
    normalAttribute->setVertexBaseType(Qt3DRender::QAttribute::Float);
    normalAttribute->setByteOffset(3 * sizeof(float));
    normalAttribute->setByteStride(3 * sizeof(glm::vec3));
    normalAttribute->setCount(_faces_transformed.size());
    normalAttribute->setName(Qt3DRender::QAttribute::defaultNormalAttributeName());

    Qt3DRender::QAttribute *colorAttribute = new Qt3DRender::QAttribute();
    colorAttribute->setAttributeType(Qt3DRender::QAttribute::VertexAttribute);
    colorAttribute->setBuffer(_vertexBuffer);
    colorAttribute->setVertexBaseType(Qt3DRender::QAttribute::Float);
    colorAttribute->setByteOffset(6 * sizeof(float));
    colorAttribute->setByteStride(3 * sizeof(glm::vec3));
    colorAttribute->setCount(_faces_transformed.size());
    colorAttribute->setName(Qt3DRender::QAttribute::defaultColorAttributeName());

    Qt3DRender::QAttribute *indexAttribute = new Qt3DRender::QAttribute();
    indexAttribute->setAttributeType(Qt3DRender::QAttribute::IndexAttribute);
    indexAttribute->setBuffer(_indexBuffer);
    colorAttribute->setVertexBaseType(Qt3DRender::QAttribute::Int);
    colorAttribute->setByteOffset(0);
    colorAttribute->setByteStride(0);
    colorAttribute->setCount(12);

    _geometry->addAttribute(positionAttribute);
    _geometry->addAttribute(normalAttribute);
    _geometry->addAttribute(colorAttribute);
    _geometry->addAttribute(indexAttribute);
}
