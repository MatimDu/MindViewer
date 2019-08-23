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
    curvebase.hpp \
    curvechart.h \
    curvegl.h \
    curvepainter.h \
    mainwindow.h \
    parser.hpp \
    thinkgearstreamparser.hpp
RESOURCES += \
    res.qrc

FORMS += \
    mainwindow.ui

ICON += icon.rc

RC_FILE += icon.rc
