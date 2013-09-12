#-------------------------------------------------
#
# Project created by QtCreator 2013-09-06T11:59:55
#
#-------------------------------------------------

QT       += widgets network xml testlib

TARGET = tst_Client
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += tst_Client.cpp \
    build.cpp \
    xmlreader.cpp \
    xmlwriter.cpp \
    management.cpp \
    network.cpp \
    buildmd5.cpp \
    protocolhandler.cpp \
    protosizecheckbuilds.cpp \
    protocol.cpp \
    protoslavecurrentbuilds.cpp \
    protoconnect.cpp \
    protocopyover.cpp \
    protogetsysinfo.cpp \
    mydiriterator.cpp \
    sysinfo.cpp \
    socketclient.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

HEADERS += \
    build.h \
    xmlreader.h \
    xmlwriter.h \
    management.h \
    network.h \
    buildmd5.h \
    protocolhandler.h \
    protosizecheckbuilds.h \
    protocol.h \
    protoslavecurrentbuilds.h \
    protoconnect.h \
    protocopyover.h \
    protogetsysinfo.h \
    mydiriterator.h \
    sysinfo.h \
    socketclient.h
