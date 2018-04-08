QT       += core gui charts
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MindViewer
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    qextserial/qextserialport.cpp \
    comwidget.cpp \
    ThinkGearStreamParser.cpp \
    highlighter.cpp

HEADERS += \
        mainwindow.h \
    globalheaders.h \
    qextserial/qextserialport.h \
    qextserial/qextserialport_global.h \
    comwidget.h \
    ThinkGearStreamParser.h \
    highlighter.h

win32 {
     SOURCES += qextserial/qextserialport_win.cpp
}

unix {
     SOURCES += qextserial/qextserialport_unix.cpp
}

RESOURCES += \
    mvicon.qrc

OTHER_FILES += mvicon.rc

RC_FILE += mvicon.rc
