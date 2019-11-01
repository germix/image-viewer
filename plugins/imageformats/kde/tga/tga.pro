
TARGET  = q_tga
VERSION = 1.0.0

TEMPLATE     = lib
CONFIG      += plugin

HEADERS += tga.h
SOURCES += tga.cpp
DISTFILES += tga.json

DESTDIR      = $(QTDIR)/plugins/imageformats
