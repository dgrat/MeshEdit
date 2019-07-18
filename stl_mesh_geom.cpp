#include <glm/glm.hpp>
#include "stl_import.h"
#include "stl_mesh_geom.h"
#include <cmath>

#include <QNode>
#include <QBuffer>
#include <QBufferDataGenerator>
#include <QAttribute>
#include <QUrl>


QByteArray createVertexData(const std::vector<stl::face> &stl_data);
QByteArray createIndexData(const std::vector<stl::face> &stl_data);

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
        const STLVertexDataFunctor *otherFunctor = functor_cast<STLVertexDataFunctor>(&other);
        if (otherFunctor != nullptr) {
            if (otherFunctor->_stl_data.size() != _stl_data.size()) return false;

            for(unsigned long i = 0; i < otherFunctor->_stl_data.size(); i++) {
                const stl::face &f1 = otherFunctor->_stl_data.at(i);
                const stl::face &f2 = _stl_data.at(i);

                if(!glm_equal(f1._vert_1, f2._vert_1)) return false;
                if(!glm_equal(f1._vert_2, f2._vert_2)) return false;
                if(!glm_equal(f1._vert_3, f2._vert_3)) return false;
            }
            return true;
        }
        return false;
    }

    QT3D_FUNCTOR(STLVertexDataFunctor)

private:
    const std::vector<stl::face> &_stl_data;
};
/*
class STLIndexDataFunctor : public Qt3DRender::QBufferDataGenerator
{
public:
    STLIndexDataFunctor(const std::vector<stl::face> &stl_data) : _stl_data(stl_data) {}

    QByteArray operator ()() override {
        return createIndexData(_stl_data);
    }

    bool operator ==(const Qt3DRender::QBufferDataGenerator &other) const override {
        const STLIndexDataFunctor *otherFunctor = functor_cast<STLIndexDataFunctor>(&other);
        if (otherFunctor != nullptr)
            return (otherFunctor->_stl_data == _stl_data);
        return false;
    }
    QT3D_FUNCTOR(STLIndexDataFunctor)

private:
    const std::vector<stl::face> &_stl_data;
};
*/
stl_mesh_geom::stl_mesh_geom(QNode *parent) : Qt3DRender::QGeometry(parent)
{

}

void stl_mesh_geom::load(const QUrl &filename) {
    stl::format stl_file(filename.path().toStdString());
    auto bbox = stl_file.estimate_bbox(stl_file.faces());
    _stl_data = stl_file.normalized(bbox);
}

void stl_mesh_geom::init() {
    m_positionAttribute = new Qt3DRender::QAttribute(this);
    m_normalAttribute = new Qt3DRender::QAttribute(this);
    m_indexAttribute = new Qt3DRender::QAttribute(this);
    m_vertexBuffer = new Qt3DRender::QBuffer(this);
    m_indexBuffer = new Qt3DRender::QBuffer(this);

    // vec3 pos, vec3 normal
    const quint32 elementSize = 3 + 3;
    const quint32 stride = elementSize * sizeof(float);
    const int faces = _stl_data.size();
    const int nVerts = faces * 3;

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
    m_normalAttribute->setByteOffset(3 * sizeof(float));
    m_normalAttribute->setCount(nVerts);

    m_indexAttribute->setAttributeType(Qt3DRender::QAttribute::IndexAttribute);
    m_indexAttribute->setVertexBaseType(Qt3DRender::QAttribute::UnsignedShort);
    m_indexAttribute->setBuffer(m_indexBuffer);
    m_indexAttribute->setCount(nVerts);

    m_vertexBuffer->setDataGenerator(QSharedPointer<STLVertexDataFunctor>::create(_stl_data));
//    m_indexBuffer->setDataGenerator(QSharedPointer<STLIndexDataFunctor>::create(_stl_data));

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
