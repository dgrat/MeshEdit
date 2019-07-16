#include <QGuiApplication>
#include <QQuickView>

#include "grid3x3.h"


int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<CtrlPtEntity>("own.classes.ctrl_points", 1, 0, "CtrlPtEntity");

    QQuickView view;

    view.resize(500, 500);
    view.setResizeMode(QQuickView::SizeRootObjectToView);
    view.setSource(QUrl("qrc:/main.qml"));
    view.show();

    return app.exec();
}
