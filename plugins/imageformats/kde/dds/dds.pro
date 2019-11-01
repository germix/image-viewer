
TARGET  = q_dds
VERSION = 1.0.0

TEMPLATE     = lib
CONFIG      += plugin

HEADERS += dds.h
SOURCES += dds.cpp
DISTFILES += dds.json

DESTDIR      = $(QTDIR)/plugins/imageformats
