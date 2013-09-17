#-------------------------------------------------
#
# Project created by QtCreator 2013-07-17T13:38:44
#
#-------------------------------------------------

QT       += core gui
QT       += network
QT       += script

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
    Machine.cpp \
    Build.cpp \
    addbuild.cpp \
    xmlWriter.cpp \
    xmlReader.cpp \
    myDirIterator.cpp \
    copybuildover.cpp \
    copycompare.cpp \
    protocolhandler.cpp \
    protoslavecurrentbuilds.cpp \
    protocol.cpp \
    protosizecheckbuilds.cpp \
    protocopyover.cpp \
    protoconnect.cpp \
    protogetsysinfo.cpp \
    protosendbuild.cpp \
    copysenderserver.cpp \
    json.cpp \
    buildmd5.cpp \
    copyqueue.cpp\
    moreinfo.cpp\
    compression.cpp \
    applicationsettings.cpp \
    copierphysical.cpp

HEADERS  += mainform.h \
    aboutversion.h \
    abouthelp.h \
    management.h \
    server.h \
    inputdialogue.h \
    Slave.h \
    Machine.h \
    Build.h \
    addbuild.h \
    xmlWriter.h \
    xmlReader.h \
    myDirIterator.h \
    serverthread.h \
    copybuildover.h \
    copycompare.h \
    protocolhandler.h \
    protoslavecurrentbuilds.h \
    protocol.h \
    protosizecheckbuilds.h \
    protocopyover.h \
    protoconnect.h \
    protogetsysinfo.h \
    protosendbuild.h \
    copysenderserver.h \
    json.h \
    buildmd5.h \
    copyqueue.h\
    moreinfo.h\
    compression.h \
    applicationsettings.h \
    copierphysical.h

FORMS    += mainform.ui \
    aboutversion.ui \
    abouthelp.ui \
    inputdialogue.ui \
    addbuild.ui \
    copybuildover.ui\
    moreinfo.ui \
    applicationsettings.ui

RESOURCES += \
    images.qrc \
    7Zip.qrc
