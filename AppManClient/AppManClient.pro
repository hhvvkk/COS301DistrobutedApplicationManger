#-------------------------------------------------
#
# Project created by QtCreator 2013-07-17T13:38:44
#
#-------------------------------------------------

QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AppManClient
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    socketclient.cpp \
    management.cpp \
    network.cpp \
    Build.cpp \
    xmlReader.cpp \
    xmlWriter.cpp \
    viewbuilds.cpp \
    sysInfo.cpp \
    addbuildfortest.cpp

HEADERS  += mainwindow.h \
    socketclient.h \
    management.h \
    network.h \
    Build.h \
    xmlReader.h \
    xmlWriter.h \
    viewbuilds.h \
    sysInfo.h \
    addbuildfortest.h

FORMS    += mainwindow.ui \
    viewbuilds.ui \
    addbuildfortest.ui

RESOURCES += \
    images.qrc
