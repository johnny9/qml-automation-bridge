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
    QObject* findQObject(QVariantMap selector);
    void startServer();

public slots:
    bool click(QVariantMap selector);

private slots:
    void handleRequest(QHttpRequest *req, QHttpResponse *resp);

private:
    void writeProperties(QVariantMap& json, QObject* object);
    void write(QVariantMap& json, QObject* object);
    void clickObject(QObject* object);
    void click(QPointF position);
    QByteArray toJson();
    QObject* m_root;
    QGuiApplication* m_app;
    QHttpServer *server;
};

#endif // QJSONBRIDGE_H
