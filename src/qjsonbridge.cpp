#include "qjsonbridge.h"

#include <qhttpserver.h>
#include <qhttprequest.h>
#include <qhttpresponse.h>

#include <QJsonObject>
#include <QJsonDocument>
#include <QDebug>
#include <QMouseEvent>
#include <QObject>
#include <QWindow>


QJsonBridge::QJsonBridge(QObject* root, QGuiApplication* app) {
    m_app = app;
    m_root = root;

    QHttpServer *server = new QHttpServer(this);
    connect(server, SIGNAL(newRequest(QHttpRequest*, QHttpResponse*)),
            this, SLOT(handleRequest(QHttpRequest*, QHttpResponse*)));

    server->listen(QHostAddress::Any, 8080);
}


void QJsonBridge::handleRequest(QHttpRequest *req, QHttpResponse *resp) {
    QRegExp exp("^/click/([a-z]+)$");
    if( exp.indexIn(req->path()) != -1 ) {
        QPointF position(150,150);
        qDebug() << "posting event";
        click(position);
    }

    findTextLocation("Hello World");

    resp->setHeader("Content-Type", "application/json");
    resp->writeHead(200);
    resp->end(toJson());
}

QByteArray QJsonBridge::toJson() {
    QVariantMap json;
    write(json, m_root);
    QJsonDocument document(QJsonObject::fromVariantMap(json));
    QByteArray output = document.toJson(QJsonDocument::Indented);

    return output;
}

QPointF QJsonBridge::findTextLocation(QString text) {
    QList<QObject*> stack;
    stack.append(m_root);
    while (!stack.isEmpty()) {
        QObject* item = stack.takeLast();
        const QList<QObject*> objList = item->children();
        QList<QObject*>::const_iterator it; 
        for (it = objList.begin(); it != objList.end(); it++) {
            stack.append(*it);
        }

        QVariantMap properties;
        writeProperties(properties, item);
        if (properties["text"] == text) {
            qDebug() << "FOUND IT";
        }
    }
}

void QJsonBridge::write(QVariantMap& parent, QObject* object) {
    const QMetaObject *metaObj = object->metaObject();
    const QString className = metaObj->className();
    QVariantMap child;
    writeProperties(child, object);
    const QList<QObject*> objList = object->children();
    QList<QObject*>::const_iterator it;
    for (it = objList.begin(); it != objList.end(); it++) {
        write(child, *it);
    }
    parent[className] = child;
}

void QJsonBridge::writeProperties(QVariantMap& json, QObject* object) {
    const QMetaObject *metaObj = object->metaObject();
    const int propertyCount = metaObj->propertyCount();
    for (int i=0; i < propertyCount; i++) {
        const QMetaProperty metaProp = metaObj->property(i);
        const QString propertyName = metaProp.name();
        json[propertyName] = object->property(propertyName.toStdString().c_str());
    }
}

void QJsonBridge::click(QPointF position) {
    QMouseEvent* eventdown = new QMouseEvent(QEvent::MouseButtonPress, position,
                                         Qt::LeftButton, Qt::LeftButton, Qt::NoModifier );
    m_app->postEvent(m_app->allWindows().first(), eventdown);
    QMouseEvent* eventup = new QMouseEvent(QEvent::MouseButtonRelease, position,
                                         Qt::LeftButton, Qt::LeftButton, Qt::NoModifier );
    m_app->postEvent(m_app->allWindows().first(), eventup);
}
