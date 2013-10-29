#-------------------------------------------------
#
# Project created by QtCreator 2013-07-17T13:38:44
#
#-------------------------------------------------

QT       += core gui
QT       += network
QT       += script

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
    addbuildfortest.cpp \
    buildmd5.cpp \
    myDirIterator.cpp \
    protocolhandler.cpp \
    protocol.cpp \
    protosizecheckbuilds.cpp \
    protoslavecurrentbuilds.cpp \
    protoconnect.cpp \
    protocopyover.cpp \
    protogetsysinfo.cpp \
    protosendbuild.cpp \
    copysenderclient.cpp \
    json.cpp \
    copierphysicalclient.cpp \
    compression.cpp \
    directoryhandler.cpp \
    protoupdatebuildinfo.cpp \
    md5thread.cpp \
    protoupdatemachineinfo.cpp \
    addapp.cpp \
    appxmlwriter.cpp \
    appxmlreader.cpp \
    protodeletebuild.cpp \
    protoapplist.cpp \
    protorunsim.cpp \
    protosendstructure.cpp

HEADERS  += mainwindow.h \
    socketclient.h \
    management.h \
    network.h \
    Build.h \
    xmlReader.h \
    xmlWriter.h \
    viewbuilds.h \
    sysInfo.h \
    addbuildfortest.h \
    buildmd5.h \
    myDirIterator.h \
    protocolhandler.h \
    protocol.h \
    protosizecheckbuilds.h \
    protoslavecurrentbuilds.h \
    protoconnect.h \
    protocopyover.h \
    protogetsysinfo.h \
    protosendbuild.h \
    copysenderclient.h \
    json.h \
    copierphysicalclient.h \
    compression.h \
    directoryhandler.h \
    protoupdatebuildinfo.h \
    md5thread.h \
    protoupdatemachineinfo.h \
    addapp.h \
    appxmlwriter.h \
    appxmlreader.h \
    protodeletebuild.h \
    protoapplist.h \
    protorunsim.h \
    protosendstructure.h


FORMS    += mainwindow.ui \
    viewbuilds.ui \
    addbuildfortest.ui \
    addapp.ui

RESOURCES += \
    images.qrc \
    7Zip.qrc \
    Styles.qrc
