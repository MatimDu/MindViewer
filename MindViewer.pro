QT       += core gui widgets

TARGET = MindViewer
TEMPLATE = app
DEFINES += QT_DEPRECATED_WARNINGS
INCLUDEPATH += $$PWD/qextserialport/
               $$PWD
SOURCES += \
        main.cpp \
        mainwindow.cpp \
    ThinkGearStreamParser.cpp \
    highlighter.cpp \
    comwidget.cpp

HEADERS += \
        mainwindow.h \
    ThinkGearStreamParser.h \
    highlighter.h \
    comwidget.h

RESOURCES += \
    res.qrc
RC_FILE += icon.rc

ICON += icon.rc
DESTDIR = bin
OBJECTS_DIR = tmp/obj
MOC_DIR = tmp/moc
RCC_DIR = tmp/rcc
UI_DIR  = tmp/ui
