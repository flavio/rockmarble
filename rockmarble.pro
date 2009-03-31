# -------------------------------------------------
# Project created by QtCreator 2009-03-24T18:06:36
# -------------------------------------------------
TARGET = rockmarble
TEMPLATE = app
INCLUDEPATH += /usr/include/marble \
    /home/flavio/hacking/testinstall/include/qjson
LIBS += -L/home/flavio/hacking/testinstall/lib/qjson \
    -lmarblewidget \
    -lqjson
SOURCES += main.cpp \
    mainwindow.cpp \
    artist.cpp \
    datafetcher.cpp \
    event.cpp \
    location.cpp \
    eventmodel.cpp \
    eventsortfilterproxymodel.cpp \
    lineedit.cpp \
    jsonconverterthread.cpp
HEADERS += mainwindow.h \
    artist.h \
    datafetcher.h \
    location.h \
    event.h \
    eventmodel.h \
    eventsortfilterproxymodel.h \
    lineedit.h \
    jsonconverterthread.h
FORMS += mainwindow.ui
QT += network
CONFIG += warn_on \
    debug_and_release
RESOURCES += resources.qrc
