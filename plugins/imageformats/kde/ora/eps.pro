QT += printsupport
TARGET  = q_ora
VERSION = 1.0.0

TEMPLATE     = lib
CONFIG      += plugin

HEADERS += eps.h
SOURCES += eps.cpp
DISTFILES += eps.json

DESTDIR      = $(QTDIR)/plugins/imageformats
