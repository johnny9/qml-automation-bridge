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
#include <QHash>
#include <QString>
#include <QMetaObject>


QJsonBridge::QJsonBridge(QObject* root, QGuiApplication* app) {
    m_app = app;
    m_root = root;
    server = new QHttpServer(this);
    connect(server, SIGNAL(newRequest(QHttpRequest*, QHttpResponse*)),
            this, SLOT(handleRequest(QHttpRequest*, QHttpResponse*)));
}

void QJsonBridge::startServer() {
    server->listen(QHostAddress::Any, 8080);
}


QVariantMap parseQuery(QString query) {
    QStringList pairs = query.split("&", QString::SkipEmptyParts);
    QVariantMap hash;
    foreach (QString str, pairs)
    {
        QString key = str.split("=")[0];
        QString value = str.split("=")[1];
        hash[key] = value;
    }
    return hash;
}

void QJsonBridge::handleRequest(QHttpRequest *req, QHttpResponse *resp) {
    QRegExp exp("^/([a-z]+)$");
    if( exp.indexIn(req->path()) != -1 ) {
        QPointF position(150,150);
        qDebug() << "posting event" << req->path();

        bool success;
        QVariantMap query = parseQuery(req->url().query());
        QString method = req->path();
        method = method.remove(0, 1);

        QMetaObject::invokeMethod(this, method.toLatin1().data(), Qt::DirectConnection,
                                  Q_RETURN_ARG(bool, success),
                                  Q_ARG(QVariantMap, query));


    }

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

bool QJsonBridge::click(QVariantMap selector) {
    qDebug() << "INVOKED!!" << selector;
    QObject* item = findQObject(selector);
    clickObject(item);
    return true;
}

bool matchesAll(QVariantMap selector, QVariantMap properties) {
    foreach(QString key, selector.keys()) {
        if (selector[key] != properties[key]) {
            return false;
        }
    }
    return true;
}

QObject* QJsonBridge::findQObject(QVariantMap selector) {
    QList<QObject*> stack;
    stack.append(m_root);
    while (!stack.isEmpty()) {
        QObject* item = stack.takeLast();
        const QList<QObject*> objList = item->children();

        QVariantMap properties;
        writeProperties(properties, item);
        if (matchesAll(selector, properties)) {
            return item;
        }

        QList<QObject*>::const_iterator it;
        for (it = objList.begin(); it != objList.end(); it++) {
            stack.append(*it);
        }
    }
    return NULL;
}

void QJsonBridge::clickObject(QObject* object) {
    if (object != NULL) {
        QVariantMap properties;
        writeProperties(properties, object);
        QPointF location;
        location.setX(properties["x"].toFloat() + properties["width"].toFloat() / 2);
        location.setY(properties["y"].toFloat() + properties["height"].toFloat() / 2);
        click(location);
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
