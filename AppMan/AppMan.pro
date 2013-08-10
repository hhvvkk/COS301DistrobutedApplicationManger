#-------------------------------------------------
#
# Project created by QtCreator 2013-07-17T13:38:44
#
#-------------------------------------------------

QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AppMan
TEMPLATE = app


SOURCES += main.cpp\
        mainform.cpp \
    aboutversion.cpp \
    abouthelp.cpp \
    management.cpp \
    serverthread.cpp \
    server.cpp \
    inputdialogue.cpp \
    Slave.cpp \
    Master.cpp \
    Machine.cpp \
    Build.cpp \
    addbuild.cpp \
    xmlWriter.cpp \
    xmlReader.cpp \
    myDirIterator.cpp

HEADERS  += mainform.h \
    aboutversion.h \
    abouthelp.h \
    management.h \
    serverthread.h \
    server.h \
    inputdialogue.h \
    Slave.h \
    Master.h \
    Machine.h \
    Build.h \
    addbuild.h \
    xmlWriter.h \
    xmlReader.h \
    myDirIterator.h

FORMS    += mainform.ui \
    aboutversion.ui \
    abouthelp.ui \
    inputdialogue.ui \
    addbuild.ui

RESOURCES += \
    images.qrc
