# -------------------------------------------------
# Project created by QtCreator 2013-09-04T20:25:30
# -------------------------------------------------

QT += testlib
QT += script
QT += network
QT -= gui


TARGET = Test
CONFIG += console
CONFIG -= app_bundle
TEMPLATE = app
SOURCES += main.cpp \
    slavetest.cpp \
    managementtest.cpp \
    machinetest.cpp
    

HEADERS += AutoTest.h \
    slavetest.h \
    managementtest.h \
    machinetest.h
