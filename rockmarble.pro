TARGET = rockmarble
DEPENDPATH += . \
    src
INCLUDEPATH += . \
    src
LIBS += -lqjson \
    -lmeegotouchcore
unix { 
    CONFIG += link_pkgconfig
    PKGCONFIG += meegotouchcore
}

#CONFIG += debug
CONFIG(debug, debug|release) {
  DEFINES += FAKE_GPS SHOW_ORIENTATION_MENU
}

QT = sql network
CONFIG += mobility
MOBILITY = location

# Input
HEADERS += src/datafetcher.h \
    src/defines.h \
    src/event.h \
    src/location.h \
    src/mainpage.h \
    src/artistitemcreator.h \
    src/eventpage.h \
    src/eventitemcreator.h \
    src/countrypage.h \
    src/countryitemcreator.h \
    src/eventdetailspage.h \
    src/lastfm.h \
    src/dbmanager.h \
    src/artistpage.h \
    src/pageitemcreator.h \
    src/pagesmodel.h \
    src/artistmodel.h \
    src/mapwidget.h \
    src/page.h \
    src/nearlocationmainpage.h \
    src/nearlocationsearchpage.h \
    src/countrymodel.h \
    src/eventmodel.h \
    src/isqlquery.h
SOURCES += src/datafetcher.cpp \
    src/event.cpp \
    src/location.cpp \
    src/main.cpp \
    src/mainpage.cpp \
    src/artistitemcreator.cpp \
    src/eventpage.cpp \
    src/eventitemcreator.cpp \
    src/countrypage.cpp \
    src/countryitemcreator.cpp \
    src/eventdetailspage.cpp \
    src/lastfm.cpp \
    src/dbmanager.cpp \
    src/artistpage.cpp \
    src/pageitemcreator.cpp \
    src/pagesmodel.cpp \
    src/artistmodel.cpp \
    src/mapwidget.cpp \
    src/nearlocationmainpage.cpp \
    src/nearlocationsearchpage.cpp \
    src/countrymodel.cpp \
    src/eventmodel.cpp \
    src/isqlquery.cpp
RESOURCES += resources.qrc

# All generated files goes to the same directory
OBJECTS_DIR = build
MOC_DIR = build
RCC_DIR = build
UI_DIR = build
DESTDIR = build

# VPATH       += src uis
# CONFIG      -=
# CONFIG      += debug
# QT          = core gui
INSTALLS += target
target.path = /usr/bin
INSTALLS += desktop
desktop.path = /usr/share/applications
desktop.files = rockmarble.desktop
INSTALLS += icon64
icon64.path = /usr/share/icons/hicolor/64x64/apps
icon64.files = icons/rockmarble.png
