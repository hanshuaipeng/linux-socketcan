#-------------------------------------------------
#
# Project created by QtCreator 2019-11-18T13:04:55
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = cantest
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    can_control.cpp \
    thredcanread.cpp

HEADERS  += mainwindow.h \
    can_control.h \
    thredcanread.h

FORMS    += mainwindow.ui
