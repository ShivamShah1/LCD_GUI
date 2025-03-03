#-------------------------------------------------
#
# Project created by QtCreator 2018-12-02T13:27:36
#
#-------------------------------------------------

QT       += core gui network serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = nitrodash
TEMPLATE = app

CONFIG += qt

SOURCES += main.cpp\
        homescreen.cpp \
    infoscreen.cpp \
    packetsscreen.cpp \
    keyboard/keyboard.cpp \
    wpa_ctrl.c \
    wpactrlifc.cpp \
    settingsscreen.cpp \
    wifiscreen.cpp \
    ethernetscreen.cpp \
    selectapscreen.cpp \
    enterpassword.cpp \
    gpsmonitor.cpp

HEADERS  += homescreen.h \
    infoscreen.h \
    packetsscreen.h \
    keyboard/keyboard.h \
    wpa_ctrl.h \
    wpactrlifc.h \
    settingsscreen.h \
    wifiscreen.h \
    ethernetscreen.h \
    selectapscreen.h \
    enterpassword.h \
    gpsmonitor.h \
    config.h

FORMS    += homescreen.ui \
    infoscreen.ui \
    packetsscreen.ui \
    settingsscreen.ui \
    keyboard/keyboard.ui \
    wifiscreen.ui \
    ethernetscreen.ui \
    selectapscreen.ui \
    enterpassword.ui

RESOURCES += \
    resources.qrc

LIBS += -L$$PWD/./ -lwpa_client

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.
