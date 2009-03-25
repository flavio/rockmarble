# -------------------------------------------------
# Project created by QtCreator 2009-03-24T18:06:36
# -------------------------------------------------
TARGET = marble
TEMPLATE = app
INCLUDEPATH += /usr/include/marble
LIBS += -lmarblewidget
SOURCES += main.cpp \
    mainwindow.cpp \
    artist.cpp \
    datafetcher.cpp
HEADERS += mainwindow.h \
    artist.h \
    datafetcher.h
FORMS += mainwindow.ui
OTHER_FILES += foo.kml \
    kde-devel-locations.kml
QT += network
