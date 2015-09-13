#include <QtTest/QtTest>
#include <QQuickView>
#include <QQuickItem>
#include <QUrl>
#include "qjsonbridge.h"

class TestTreeSearch: public QObject
{
    Q_OBJECT
    
private slots:
    void initTestCase();
    void toUpper();
    void cleanup();

private:
   QQuickView view;
   QJsonBridge* bridge;
};

void TestTreeSearch::initTestCase()
{
    view.setSource(QUrl(QStringLiteral("qml/test.qml")));
    bridge = new QJsonBridge(view.rootObject(), NULL);
}

void TestTreeSearch::toUpper()
{
    QString str = "Hello";
    QVERIFY(str.toUpper() == "HELLO");
//    QPointF location = bridge->findTextLocation("Hello World");
//    qDebug() << location;
}


void TestTreeSearch::cleanup()
{
    delete bridge;
}

QTEST_MAIN(TestTreeSearch)
#include "treetest.moc"
