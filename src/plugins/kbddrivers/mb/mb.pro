TARGET = mbkbddriver
include(../../qpluginbase.pri)

QTDIR_build:DESTDIR = $$QT_BUILD_TREE/plugins/kbddrivers
target.path = $$[QT_INSTALL_PLUGINS]/kbddrivers
INSTALLS += target

HEADERS	= $$QT_SOURCE_TREE/src/gui/embedded/qkbdlinuxinput_qws.h

SOURCES	= main.cpp \
	$$QT_SOURCE_TREE/src/gui/embedded/qkbdlinuxinput_qws.cpp

HEADERS += mbkbdhandler.h
SOURCES += mbkbdhandler.cpp
