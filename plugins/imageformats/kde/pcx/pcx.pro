
TARGET  = q_pcx
VERSION = 1.0.0

TEMPLATE     = lib
CONFIG      += plugin

HEADERS += pcx_p.h
SOURCES += pcx.cpp
DISTFILES += pcx.json

DESTDIR      = $(QTDIR)/plugins/imageformats
