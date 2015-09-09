TEMPLATE = lib

TARGET = qjsonbridge

QT += network

LIBS += -lqhttpserver

HEADERS = *.h
SOURCES = *.cpp

OBJECTS_DIR = ../build
MOC_DIR = ../build
DESTDIR = ../lib

target.path = /usr/local/lib
headers.path = /usr/local/include
headers.files = $$HEADERS
INSTALLS += target headers
