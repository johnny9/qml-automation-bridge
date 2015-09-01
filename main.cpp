#include <QtGui/QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickItem>
#include <QObject>
#include <QQuickView>
#include "qjsonbridge.h"

QJsonBridge* bridge;

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQuickView view;
    view.setSource(QUrl(QStringLiteral("qml/jsonbrsge2/main.qml")));
    bridge = new QJsonBridge(view.rootObject(), &app);
    return app.exec();
}
