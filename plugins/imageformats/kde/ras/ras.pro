
TARGET  = q_ras
VERSION = 1.0.0

TEMPLATE     = lib
CONFIG      += plugin

HEADERS += ras.h
SOURCES += ras.cpp
DISTFILES += ras.json

DESTDIR      = $(QTDIR)/plugins/imageformats
