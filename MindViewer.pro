QT       += core gui widgets

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
    mainwindow.cpp

HEADERS += \
        mainwindow.h \
    ThinkGearStreamParser.h \
    qextserialport/qextserialbase.h \
    qextserialport/qextserialport.h \
    mainwindow.h
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
    mainwindow.ui
