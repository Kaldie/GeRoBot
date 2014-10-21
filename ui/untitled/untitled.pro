#-------------------------------------------------
#
# Project created by QtCreator 2014-10-19T23:53:24
#
#-------------------------------------------------

QT       += widgets

TARGET = untitled
TEMPLATE = lib

DEFINES += UNTITLED_LIBRARY

SOURCES += UserInterface.cpp

HEADERS += UserInterface.h\
        untitled_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
