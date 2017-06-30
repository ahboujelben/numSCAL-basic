#-------------------------------------------------
#
# Project created by QtCreator 2014-01-02T17:45:02
# Author:      Ahmed Hamdi Boujelben <ahmed.hamdi.boujelben@gmail.com>
# Copyright:   (c) 2017 Ahmed Hamdi Boujelben
# Licence:     Attribution 4.0 International (CC BY 4.0)
#
#-------------------------------------------------

QT       += core

QT       += gui

QT       += opengl

QT       += printsupport

TARGET = numSCAL
CONFIG   += console
CONFIG   -= app_bundle
CONFIG += c++11

TEMPLATE = app

##Use this libraries if you use Linux
#LIBS += -lGLU

##Use these libraries if you use Windows
LIBS += -lopengl32 -lglu32

SOURCES += main.cpp \
    pore.cpp \
    node.cpp \
    network.cpp \
    mainwindow.cpp \
    cluster.cpp \
    worker.cpp \
    widget3d.cpp \
    hoshenKopelmann.cpp \
    loadData.cpp \
    element.cpp \
    solver.cpp \
    generationRegular.cpp \
    generationExtracted.cpp \
    misc.cpp \
    qcustomplot.cpp \
    films.cpp \
    twoPhaseFlow.cpp \
    twoPhaseFlowPT.cpp

HEADERS += \
    pore.h \
    node.h \
    network.h \
    tools.h \
    mainwindow.h \
    widget3d.h \
    cluster.h \
    worker.h \
    element.h \
    qcustomplot.h

OTHER_FILES +=

FORMS += \
    mainwindow.ui

CONFIG += warn_off

RESOURCES += \
    resources.qrc
