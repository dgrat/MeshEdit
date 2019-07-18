#pragma once

#include <QGeometryRenderer>

class QNode;
class QUrl;


class stl_mesh : public Qt3DRender::QGeometryRenderer
{
    Q_OBJECT

public:
    stl_mesh(QNode *parent = nullptr);

public slots:
    void sl_load(const QUrl &filename);
};
