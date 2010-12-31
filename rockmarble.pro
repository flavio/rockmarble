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

# Input
HEADERS += src/datafetcher.h \
    src/defines.h \
    src/event.h \
    src/eventmodel.h \
    src/eventsortfilterproxymodel.h \
    src/lineedit.h \
    src/location.h \
    src/mainpage.h \
    src/artistitemcreator.h \
    src/eventpage.h \
    src/eventitemcreator.h \
    src/countrypage.h \
    src/countryitemcreator.h \
    src/eventdetailspage.h

# FORMS += src/mainwindow.ui
SOURCES += src/datafetcher.cpp \
    src/event.cpp \
    src/eventmodel.cpp \
    src/eventsortfilterproxymodel.cpp \
    src/lineedit.cpp \
    src/location.cpp \
    src/main.cpp \
    src/mainpage.cpp \
    src/artistitemcreator.cpp \
    src/eventpage.cpp \
    src/eventitemcreator.cpp \
    src/countrypage.cpp \
    src/countryitemcreator.cpp \
    src/eventdetailspage.cpp
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
INSTALLS += service
service.path = /usr/share/dbus-1/services
service.files = rockmarble.service
INSTALLS += icon64
icon64.path = /usr/share/icons/hicolor/64x64/apps
icon64.files = icons/rockmarble.png

# Targets for debian source and binary package creation
debian-src.commands = dpkg-buildpackage \
    -S \
    -r \
    -us \
    -uc \
    -d
debian-bin.commands = dpkg-buildpackage \
    -b \
    -r \
    -uc \
    -d
debian-all.depends = debian-src \
    debian-bin

# Clean all but Makefile
compiler_clean.commands = -$(DEL_FILE) \
    $(TARGET)
QMAKE_EXTRA_TARGETS += debian-all \
    debian-src \
    debian-bin \
    compiler_clean
OTHER_FILES += qml/main.qml
