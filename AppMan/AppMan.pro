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
    aboutversion.cpp \
    abouthelp.cpp

HEADERS  += mainform.h \
    aboutversion.h \
    abouthelp.h

FORMS    += mainform.ui \
    aboutversion.ui \
    abouthelp.ui

RESOURCES += \
    images.qrc