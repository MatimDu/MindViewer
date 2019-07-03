QT       += core gui widgets sql

TARGET = MindViewer
TEMPLATE = app
DEFINES += QT_DEPRECATED_WARNINGS
INCLUDEPATH += $$PWD/qextserialport
DESTDIR = bin
OBJECTS_DIR = tmp/obj
MOC_DIR = tmp/moc
RCC_DIR = tmp/rcc
UI_DIR  = tmp/ui

SOURCES += \
        main.cpp \
    ThinkGearStreamParser.cpp \
    qextserialport/qextserialbase.cpp \
    qextserialport/qextserialport.cpp \
    COM.cpp \
    Widget.cpp \
    Data.cpp

HEADERS += \
    ThinkGearStreamParser.h \
    qextserialport/qextserialbase.h \
    qextserialport/qextserialport.h \
    COM.h \
    Widget.h \
    Data.h
win32:{
HEADERS += $$PWD/qextserialport/win_qextserialport.h
SOURCES += $$PWD/qextserialport/win_qextserialport.cpp
}
unix:{
HEADERS += $$PWD/qextserialport/posix_qextserialport.h
SOURCES += $$PWD/qextserialport/posix_qextserialport.cpp
}
RESOURCES += \
    res.qrc

FORMS += \
    COM.ui \
    Widget.ui
