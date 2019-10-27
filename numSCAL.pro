#-------------------------------------------------
#
# Project created by QtCreator 2014-01-02T17:45:02
#
#-------------------------------------------------

QT       += core

QT       += gui

QT       += opengl

QT       += printsupport

TARGET = numSCAL
CONFIG   += console
CONFIG   -= app_bundle
CONFIG += c++14

TEMPLATE = app

win32 {
    contains(QT_ARCH, i386) {
        #32 bit
        LIBS += -lopengl32 $$PWD/libs/glew/bin/Release/Win32/glew32.dll
        LIBS += -L$$PWD/libs/glew/lib/Release/Win32/ -lglew32
    } else {
        #64 bit
        LIBS += -lopengl32 $$PWD/libs/glew/bin/Release/x64/glew32.dll
        LIBS += -L$$PWD/libs/glew/lib/Release/x64/ -lglew32
    }
}

unix {
    LIBS += -lGLEW
}


SOURCES += main.cpp \
    builders/networkbuilder.cpp \
    builders/numscalNetworkBuilder.cpp \
    builders/regularNetworkBuilder.cpp \
    builders/statoilNetworkBuilder.cpp \
    gui/mainwindow.cpp \
    gui/widget3d.cpp \
    misc/randomGenerator.cpp \
    misc/scopedtimer.cpp \
    misc/shader.cpp \
    misc/tools.cpp \
    misc/userInput.cpp \
    network/cluster.cpp \
    network/element.cpp \
    network/networkmodel.cpp \
    network/node.cpp \
    network/pore.cpp \
    operations/hkClustering.cpp \
    operations/pnmOperation.cpp \
    operations/pnmSolver.cpp \
    simulations/steady-state-cycle/forcedWaterInjection.cpp \
    simulations/steady-state-cycle/primaryDrainage.cpp \
    simulations/steady-state-cycle/secondaryOilDrainage.cpp \
    simulations/steady-state-cycle/spontaneousImbibtion.cpp \
    simulations/steady-state-cycle/spontaneousOilInvasion.cpp \
    simulations/steady-state-cycle/steadyStateSimulation.cpp \
    simulations/tracer-flow/tracerFlowSimulation.cpp \
    simulations/unsteady-state-flow/unsteadyStateSimulation.cpp \
    simulations/template-simulation/templateFlowSimulation.cpp \
    simulations/simulation.cpp \
    simulations/renderer/renderer.cpp \
    misc/maths.cpp \
    libs/qcustomplot/qcustomplot.cpp


HEADERS += \
    builders/networkbuilder.h \
    builders/numscalNetworkBuilder.h \
    builders/regularNetworkBuilder.h \
    builders/statoilNetworkBuilder.h \
    gui/mainwindow.h \
    gui/widget3d.h \
    misc/maths.h \
    misc/randomGenerator.h \
    misc/scopedtimer.h \
    misc/shader.h \
    misc/tools.h \
    misc/userInput.h \
    network/cluster.h \
    network/element.h \
    network/iterator.h \
    network/networkmodel.h \
    network/node.h \
    network/pore.h \
    operations/hkClustering.h \
    operations/pnmOperation.h \
    operations/pnmSolver.h \
    simulations/steady-state-cycle/forcedWaterInjection.h \
    simulations/steady-state-cycle/primaryDrainage.h \
    simulations/steady-state-cycle/secondaryOilDrainage.h \
    simulations/steady-state-cycle/spontaneousImbibtion.h \
    simulations/steady-state-cycle/spontaneousOilInvasion.h \
    simulations/steady-state-cycle/steadyStateSimulation.h \
    simulations/tracer-flow/tracerFlowSimulation.h \
    simulations/unsteady-state-flow/unsteadyStateSimulation.h \
    simulations/template-simulation/templateFlowSimulation.h \
    simulations/simulation.h \
    simulations/renderer/renderer.h \
    libs/qcustomplot/qcustomplot.h


INCLUDEPATH += \
    network \
    builders \
    simulations \
    simulations/steady-state-cycle \
    simulations/tracer-flow \
    simulations/unsteady-state-flow \
    operations \
    gui \
    misc \
    libs \
    resources

FORMS += \
    gui/mainwindow.ui

CONFIG += warn_off

RESOURCES += \
    resources/resources.qrc
