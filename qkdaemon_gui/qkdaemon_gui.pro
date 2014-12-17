#-------------------------------------------------
#
# Project created by QtCreator 2014-12-13T20:38:21
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qkdaemon_gui
TEMPLATE = app

INCLUDEPATH += ../utils/

SOURCES += main.cpp\
        mainwindow.cpp \
    ../utils/qkgui.cpp \
    qkconnectworker.cpp \
    aboutdialog.cpp

HEADERS  += mainwindow.h \
    ../utils/qkgui.h \
    qkdaemon_gui.h \
    qkconnectworker.h \
    aboutdialog.h

FORMS    += mainwindow.ui \
    aboutdialog.ui

CONFIG(debug, debug|release){
    DESTDIR = debug
} else {
    DESTDIR = release
}

OBJECTS_DIR = obj
MOC_DIR = moc
RCC_DIR = rcc
UI_DIR = ui

RESOURCES += \
    resources/img.qrc
