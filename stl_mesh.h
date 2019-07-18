#pragma once

#include <QGeometryRenderer>

class QNode;
class QUrl;


class stl_mesh : public Qt3DRender::QGeometryRenderer
{
    Q_OBJECT

public:
    stl_mesh(QNode *parent = nullptr);

private:
    // As this is a default provided geometry renderer, no one should be able
    // to modify the QGeometryRenderer's properties
    void setVertexCount(int vertexCount);
    void setIndexOffset(int indexOffset);
    void setFirstInstance(int firstInstance);
    void setRestartIndexValue(int index);
    void setPrimitiveRestartEnabled(bool enabled);
    void setGeometry(Qt3DRender::QGeometry *geometry);
    void setPrimitiveType(PrimitiveType primitiveType);
};
