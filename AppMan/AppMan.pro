#-------------------------------------------------
#
# Project created by QtCreator 2013-07-17T13:38:44
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AppMan
TEMPLATE = app


SOURCES += main.cpp\
        mainform.cpp \
    build.cpp \
    aboutversion.cpp

HEADERS  += mainform.h \
    build.h \
    aboutversion.h

FORMS    += mainform.ui \
    aboutversion.ui

RESOURCES += \
    images.qrc
