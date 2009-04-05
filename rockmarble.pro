# -------------------------------------------------
# Project created by QtCreator 2009-03-24T18:06:36
# -------------------------------------------------
TARGET = rockmarble
TEMPLATE = app
INCLUDEPATH += /usr/include/marble \
    /usr/include/qjson
LIBS += -L/usr/lib/qjson \
    -L/usr/lib64/qjson \
    -lmarblewidget \
    -lqjson
SOURCES += main.cpp \
    mainwindow.cpp \
    datafetcher.cpp \
    event.cpp \
    location.cpp \
    eventmodel.cpp \
    eventsortfilterproxymodel.cpp \
    lineedit.cpp
HEADERS += mainwindow.h \
    datafetcher.h \
    location.h \
    event.h \
    eventmodel.h \
    eventsortfilterproxymodel.h \
    lineedit.h \
    defines.h
FORMS += mainwindow.ui
QT += network
CONFIG += warn_on \
    debug_and_release
RESOURCES += resources.qrc
target.path += /usr/bin/
INSTALLS += target
