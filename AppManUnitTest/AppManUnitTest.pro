# -------------------------------------------------
# Project created by QtCreator 2013-09-04T20:25:30
# -------------------------------------------------

QT += testlib
QT += script
QT += network
QT += core gui
QT += network
QT += script
QT += sql

SUBDIRS = AppMan
TARGET = Test
CONFIG += console
CONFIG -= app_bundle
TEMPLATE = app
SOURCES += main.cpp \
    slavetest.cpp \
    managementtest.cpp \
    machinetest.cpp \
    AppMan/Build.cpp \
    AppMan/Slave.cpp \
    AppMan/Machine.cpp \
    AppMan/protocolhandler.cpp \
    AppMan/protocol.cpp \
    AppMan/protoslavecurrentbuilds.cpp \
    AppMan/protoconnect.cpp \
    AppMan/protocopyover.cpp \
    AppMan/protogetsysinfo.cpp \
    AppMan/protosendbuild.cpp \
    AppMan/protosizecheckbuilds.cpp \
    AppMan/protoupdatebuildinfo.cpp \
    AppMan/protoupdatemachineinfo.cpp \#######EXTRA####
    AppMan/abouthelp.cpp \
    AppMan/aboutversion.cpp \
    AppMan/addbuild.cpp \
    AppMan/addsimulation.cpp \
    AppMan/buildmd5.cpp \
    AppMan/compression.cpp \
    AppMan/copierphysical.cpp \
    AppMan/copybuildover.cpp \
    AppMan/copycompare.cpp \
    AppMan/copyqueue.cpp \
    AppMan/copysenderserver.cpp \
    AppMan/database.cpp \
    AppMan/directoryhandler.cpp \
    AppMan/inputdialogue.cpp \
    AppMan/json.cpp \
    AppMan/mainbuildinfo.cpp \
    AppMan/management.cpp \
    AppMan/md5thread.cpp \
    AppMan/moreinfo.cpp \
    AppMan/myDirIterator.cpp \
    AppMan/server.cpp \
    AppMan/serverthread.cpp \
    AppMan/simulation.cpp \
    AppMan/xmlReader.cpp \
    AppMan/xmlWriter.cpp \
    buildtest.cpp \
    protocoltest.cpp



HEADERS += AutoTest.h \
    slavetest.h \
    managementtest.h \
    machinetest.h \
    AppMan/Build.h \
    AppMan/Slave.h \
    AppMan/Machine.h \
    AppMan/protocolhandler.h \
    AppMan/protoslavecurrentbuilds.h \
    AppMan/protocol.h \
    AppMan/protoconnect.h \
    AppMan/protocopyover.h \
    AppMan/protogetsysinfo.h \
    AppMan/protosendbuild.h \
    AppMan/protosizecheckbuilds.h \
    AppMan/protoupdatebuildinfo.h \
    AppMan/protoupdatemachineinfo.h \#######EXTRA####
    AppMan/abouthelp.h \
    AppMan/aboutversion.h \
    AppMan/addbuild.h \
    AppMan/addsimulation.h \
    AppMan/buildmd5.h \
    AppMan/compression.h \
    AppMan/copierphysical.h \
    AppMan/copybuildover.h \
    AppMan/copycompare.h \
    AppMan/copyqueue.h \
    AppMan/copysenderserver.h \
    AppMan/database.h \
    AppMan/directoryhandler.h \
    AppMan/inputdialogue.h \
    AppMan/json.h \
    AppMan/mainbuildinfo.h \
    AppMan/management.h \
    AppMan/md5thread.h \
    AppMan/moreinfo.h \
    AppMan/myDirIterator.h \
    AppMan/server.h \
    AppMan/serverthread.h \
    AppMan/simulation.h \
    AppMan/xmlReader.h \
    AppMan/xmlWriter.h \
    buildtest.h \
    protocoltest.h


OTHER_FILES +=

FORMS    += AppMan/mainform.ui \
    AppMan/aboutversion.ui \
    AppMan/abouthelp.ui \
    AppMan/inputdialogue.ui \
    AppMan/addbuild.ui \
    AppMan/copybuildover.ui\
    AppMan/moreinfo.ui \
    AppMan/applicationsettings.ui \
    AppMan/addsimulation.ui

RESOURCES += \
    AppMan/images.qrc \
    AppMan/7Zip.qrc
