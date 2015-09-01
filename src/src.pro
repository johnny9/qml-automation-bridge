TEMPLATE = lib

TARGET = qjsonbridge

QT += network

LIBS += -lqhttpserver

CONFIG += dll debug_and_release

HEADERS = qjsonbridge.h
SOURCES = *.cpp

OBJECTS_DIR = ../build
MOC_DIR = ../build
DESTDIR = ../lib


