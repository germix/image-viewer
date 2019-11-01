
TARGET  = q_xcf
VERSION = 1.0.0

TEMPLATE     = lib
CONFIG      += plugin

HEADERS += xcf.h \
    gimp.h
SOURCES += xcf.cpp
DISTFILES += xcf.json

DESTDIR      = $(QTDIR)/plugins/imageformats
