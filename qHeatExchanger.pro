#-------------------------------------------------
#
# Project created by QtCreator 2015-06-24T08:59:06
#
#-------------------------------------------------

QT       += core gui printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qHeatExchanger
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    qwt_mainwindow.cpp \
    output_module.cpp \
    algorythm.cpp \
    controller.cpp \
    oweninputmodule.cpp

HEADERS  += mainwindow.h \
    qwt_mainwindow.h \
    output_module.h \
    algorythm.h \
    controller.h \
    oweninputmodule.h

FORMS    += mainwindow.ui

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/qwt/ -lqwt
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/qwt/ -lqwtd

INCLUDEPATH += $$PWD/qwt
DEPENDPATH += $$PWD/qwt

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/modbus/ -llibmodbus
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/modbus/ -llibmodbusd

INCLUDEPATH += $$PWD/modbus
DEPENDPATH += $$PWD/modbus
