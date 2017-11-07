#-------------------------------------------------
#
# Project created by QtCreator 2017-11-06T13:52:07
#
#-------------------------------------------------

QT       += core gui
CONFIG += c++1z
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TeQtSimplePaint
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    matrix.cpp \
    model.cpp

HEADERS  += mainwindow.h \
    matrix.h \
    model.h

FORMS    += mainwindow.ui
