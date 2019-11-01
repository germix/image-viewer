
TARGET  = q_psd
VERSION = 1.0.0

TEMPLATE     = lib
CONFIG      += plugin

HEADERS += psd.h
SOURCES += psd.cpp
DISTFILES += psd.json

DESTDIR      = $(QTDIR)/plugins/imageformats
