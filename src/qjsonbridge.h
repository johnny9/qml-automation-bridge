#ifndef QJSONBRIDGE_H
#define QJSONBRIDGE_H
#include <qhttpserverfwd.h>

#include <QObject>
#include <QGuiApplication>
#include <QByteArray>
#include <QVariantMap>

class QJsonBridge : public QObject {
    Q_OBJECT

public:
    QJsonBridge(QObject* root, QGuiApplication* app);

private slots:
    void handleRequest(QHttpRequest *req, QHttpResponse *resp);

private:
    void writeProperties(QVariantMap& json, QObject* object);
    void write(QVariantMap& json, QObject* object);
    QByteArray toJson();
    QObject* m_root;
    QGuiApplication* m_app;

    void click(QPointF position);
};

#endif // QJSONBRIDGE_H
