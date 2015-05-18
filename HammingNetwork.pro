#-------------------------------------------------
#
# Project created by QtCreator 2015-05-18T22:31:00
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QMAKE_CXXFLAGS += -std=c++0x
TARGET = HammingNetwork
TEMPLATE = app


SOURCES += main.cpp\
    hammingnetwork.cpp \
    project.cpp \
    window.cpp

HEADERS  += \
    hammingnetwork.h \
    project.h \
    window.h

RESOURCES += \
    resources.qrc

FORMS += \
    window.ui
