#-------------------------------------------------
#
# Project created by QtCreator 2015-08-19T13:36:02
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LSysMonitor
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    memoryinfopage.cpp \
    processinfopage.cpp \
    moduleinfopage.cpp \
    systeminfopage.cpp \
    aboutpage.cpp

HEADERS  += widget.h \
    memoryinfopage.h \
    processinfopage.h \
    moduleinfopage.h \
    systeminfopage.h \
    aboutpage.h
