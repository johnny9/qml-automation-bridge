QT += testlib quick

CONFIG += testcase

LIBS += -lqhttpserver

HEADERS = ../src/*.h
SOURCES = treetest.cpp ../src/*.cpp

run_test.target = all
run_test.commands = ./test
QMAKE_EXTRA_TARGETS += run_test

OTHER_FILES += \
    qml/test.qml
