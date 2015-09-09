TEMPLATE = app

SOURCES += main.cpp

QT += quick

TARGET = example

INCLUDEPATH += ../src
LIBS += -L../lib

LIBS += -lqjsonbridge

OTHER_FILES += \
    qml/main.qml

