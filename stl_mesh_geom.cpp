#include <glm/glm.hpp>
#include "stl_mesh_geom.h"
#include <cmath>

#include <QNode>
#include <QBuffer>
#include <QBufferDataGenerator>
#include <QAttribute>
#include <QUrl>

namespace {
    QByteArray createVertexData(const std::vector<stl::face> &stl_data) {
        QByteArray mesh_buf;
        mesh_buf.resize(stl_data.size() * 6 * sizeof(glm::vec3));
        glm::vec3 *mesh_buf_acc = reinterpret_cast<glm::vec3 *>(mesh_buf.data());

        uint32_t vert_ind = 0;
        for (auto &face : stl_data) {
            mesh_buf_acc[vert_ind++] = face._vert_1;
            mesh_buf_acc[vert_ind++] = face._norm;

            mesh_buf_acc[vert_ind++] = face._vert_2;
            mesh_buf_acc[vert_ind++] = face._norm;

            mesh_buf_acc[vert_ind++] = face._vert_3;
            mesh_buf_acc[vert_ind++] = face._norm;
        }
        return mesh_buf;
    }

    QByteArray createIndexData(const std::vector<stl::face> &stl_data) {
        QByteArray index_buf;
        index_buf.resize(stl_data.size() * 3 * sizeof(uint32_t));
        uint32_t *index_buf_acc = reinterpret_cast<uint32_t *>(index_buf.data());

        for (unsigned int i = 0; i < stl_data.size()*3; i++) {
            index_buf_acc[i] = i;
        }

        return index_buf;
    }

    template <typename T>
    void ignore(T &&)
    { }
};

inline bool glm_equal(const glm::vec3 &v1, const glm::vec3 &v2) {
    glm::vec3 r = glm::abs(v1 - v2);
    if(r.x > FLT_EPSILON) return false;
    if(r.y > FLT_EPSILON) return false;
    if(r.z > FLT_EPSILON) return false;
    return true;
}

class STLVertexDataFunctor : public Qt3DRender::QBufferDataGenerator
{
public:
    STLVertexDataFunctor(const std::vector<stl::face> &stl_data) : _stl_data(stl_data) {}

    QByteArray operator ()() override {
        return createVertexData(_stl_data);
    }

    bool operator ==(const Qt3DRender::QBufferDataGenerator &other) const override {
        ignore(other);
        return false;
    }

    QT3D_FUNCTOR(STLVertexDataFunctor)

private:
    const std::vector<stl::face> &_stl_data;
};

class STLIndexDataFunctor : public Qt3DRender::QBufferDataGenerator
{
public:
    STLIndexDataFunctor(const std::vector<stl::face> &stl_data) : _stl_data(stl_data) {}

    QByteArray operator ()() override {
        return createIndexData(_stl_data);
    }

    bool operator ==(const Qt3DRender::QBufferDataGenerator &other) const override {
        ignore(other);
        return false;
    }
    QT3D_FUNCTOR(STLIndexDataFunctor)

private:
    const std::vector<stl::face> &_stl_data;
};

stl_mesh_geom::stl_mesh_geom(QNode *parent) : Qt3DRender::QGeometry(parent)
{

}

void stl_mesh_geom::load(const QUrl &filename) {
    if(filename.isEmpty()) return;

    stl::format stl_file(filename.path().toStdString());
    auto bbox = stl_file.estimate_bbox(stl_file.faces());
    _stl_data = stl_file.normalized(bbox);

    bbox = stl::format::estimate_bbox(_stl_data);
    _dim = bbox._max - bbox._min;

    if(_stl_data.size() > 0) {
        init(_stl_data);
    }
}

void stl_mesh_geom::refresh(const std::vector<stl::face> &data) {
    if(data.size() > 0) {
        init(data);
    }
}

void stl_mesh_geom::init(const std::vector<stl::face> &data) {
    if(!m_positionAttribute) m_positionAttribute = new Qt3DRender::QAttribute(this);
    if(!m_normalAttribute) m_normalAttribute = new Qt3DRender::QAttribute(this);
    if(!m_indexAttribute) m_indexAttribute = new Qt3DRender::QAttribute(this);
    if(!m_vertexBuffer) m_vertexBuffer = new Qt3DRender::QBuffer(this);
    if(!m_indexBuffer) m_indexBuffer = new Qt3DRender::QBuffer(this);

    // vec3 pos, vec3 normal
    const quint32 elementSize = 1 + 1;
    const quint32 stride = elementSize * sizeof(glm::vec3);
    const size_t faces = data.size();
    const size_t nVerts = faces * 3;

    m_positionAttribute->setName(Qt3DRender::QAttribute::defaultPositionAttributeName());
    m_positionAttribute->setVertexBaseType(Qt3DRender::QAttribute::Float);
    m_positionAttribute->setVertexSize(3);
    m_positionAttribute->setAttributeType(Qt3DRender::QAttribute::VertexAttribute);
    m_positionAttribute->setBuffer(m_vertexBuffer);
    m_positionAttribute->setByteStride(stride);
    m_positionAttribute->setCount(nVerts);

    m_normalAttribute->setName(Qt3DRender::QAttribute::defaultNormalAttributeName());
    m_normalAttribute->setVertexBaseType(Qt3DRender::QAttribute::Float);
    m_normalAttribute->setVertexSize(3);
    m_normalAttribute->setAttributeType(Qt3DRender::QAttribute::VertexAttribute);
    m_normalAttribute->setBuffer(m_vertexBuffer);
    m_normalAttribute->setByteStride(stride);
    m_normalAttribute->setByteOffset(sizeof(glm::vec3));
    m_normalAttribute->setCount(nVerts);

    m_indexAttribute->setAttributeType(Qt3DRender::QAttribute::IndexAttribute);
    m_indexAttribute->setVertexBaseType(Qt3DRender::QAttribute::UnsignedInt);
    m_indexAttribute->setBuffer(m_indexBuffer);
    m_indexAttribute->setCount(nVerts);

    m_vertexBuffer->setDataGenerator(QSharedPointer<STLVertexDataFunctor>::create(data));
    m_indexBuffer->setDataGenerator(QSharedPointer<STLIndexDataFunctor>::create(data));

    this->addAttribute(m_positionAttribute);
    this->addAttribute(m_normalAttribute);
    this->addAttribute(m_indexAttribute);
}

Qt3DRender::QAttribute *stl_mesh_geom::positionAttribute() const {
    return m_positionAttribute;
}

Qt3DRender::QAttribute *stl_mesh_geom::normalAttribute() const {
    return m_normalAttribute;
}

Qt3DRender::QAttribute *stl_mesh_geom::indexAttribute() const {
    return m_indexAttribute;
}
