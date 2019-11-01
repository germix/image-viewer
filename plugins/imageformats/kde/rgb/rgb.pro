
TARGET  = q_rgb
VERSION = 1.0.0

TEMPLATE     = lib
CONFIG      += plugin

HEADERS += rgb.h
SOURCES += rgb.cpp
DISTFILES += rgb.json

DESTDIR      = $(QTDIR)/plugins/imageformats
