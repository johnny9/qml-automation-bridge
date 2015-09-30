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
        value.replace("+", " ");
        hash[key] = value;
    }
    return hash;
}

void QJsonBridge::handleRequest(QHttpRequest *req, QHttpResponse *resp) {
    QRegExp exp("^/([a-zA-Z]+)$");
    if( exp.indexIn(req->path()) != -1 ) {
        QPointF position(150,150);

        bool success;
        QVariantMap query = parseQuery(req->url().query());
        QString method = req->path();
        method = method.remove(0, 1);
        QVariantMap resultItem;

        QMetaObject::invokeMethod(this, method.toLatin1().data(), Qt::DirectConnection,
                                  Q_RETURN_ARG(bool, success),
                                  Q_ARG(QVariantMap, query),
                                  Q_ARG(QVariantMap&, resultItem));


        if (success) {
            resp->setHeader("Content-Type", "application/json");
            resp->writeHead(200);
            QJsonDocument document(QJsonObject::fromVariantMap(resultItem));
            resp->end(document.toJson(QJsonDocument::Indented));
        } else {
            resp->writeHead(400);
            resp->end();
        }
    } else {
        resp->setHeader("Content-Type", "application/json");
        resp->writeHead(200);
        resp->end(toJson());
        qDebug() << "bad function";
    }
}

QByteArray QJsonBridge::toJson() {
    QVariantMap json;
    write(json, m_root);
    QJsonDocument document(QJsonObject::fromVariantMap(json));
    QByteArray output = document.toJson(QJsonDocument::Indented);

    return output;
}

bool QJsonBridge::click(QVariantMap selector, QVariantMap& resultItem) {
    //Only click objects that are vidible
    selector["visible"] = true;
    QObject* item = findQObject(selector, m_root);
    if (item == NULL) {
        return false;
    }

    clickObject(item);
    write(resultItem, item);
    return true;
}

bool QJsonBridge::findItem(QVariantMap selector, QVariantMap& resultItem) {
   selector["visible"] = true;
   QObject* item = findQObject(selector, m_root);
   if (item == NULL) {
       return false;
   }

   write(resultItem, item);
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
    QList<QObject*> stack(m_root->children());
    while (!stack.isEmpty()) {
        QObject* item = stack.takeLast();
        const QList<QObject*> objList = item->children();

        QVariantMap properties;
        writeProperties(properties, item);
        if (matchesAll(selector, properties)) {
            stack.clear();
            return item;
        }

        QList<QObject*>::const_iterator it;
        for (it = objList.begin(); it != objList.end(); it++) {
            stack.append(*it);
        }
    }
    stack.clear();
    return NULL;
}

QObject* QJsonBridge::findQObject(QVariantMap selector, QObject* object) {
    const QList<QObject*> objList = object->children();
    QList<QObject*>::const_iterator it;
    for (it = objList.begin(); it != objList.end(); it++) {
        QObject* result = findQObject(selector, *it);
        if (result != NULL) {
            return result;
        }
    }
   
    QVariantMap properties;
    writeProperties(properties, object);
    if (matchesAll(selector, properties)) {
        return object;
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

    if (parent.contains(className)) {
        QVariant i = "1";
        while(parent.contains(className + i.toString())) {
            i = i.toInt() + 1;
        }
        parent[className+i.toString()] = child;
    } else {
        parent[className] = child;
    }
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
