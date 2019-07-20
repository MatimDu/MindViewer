QT       += core gui widgets sql

TARGET = MindViewer
TEMPLATE = app
DEFINES += QT_DEPRECATED_WARNINGS
DESTDIR = bin
OBJECTS_DIR = tmp/obj
MOC_DIR = tmp/moc
RCC_DIR = tmp/rcc
UI_DIR  = tmp/ui

SOURCES += \
        main.cpp \
    mainwindow.cpp

HEADERS += \
    mainwindow.h \
    thinkgearstreamparser.hpp
RESOURCES += \
    res.qrc

FORMS += \
    mainwindow.ui
