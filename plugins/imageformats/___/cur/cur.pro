
TARGET  = q_cur
VERSION = 1.0.0

TEMPLATE     = lib
CONFIG      += plugin

HEADERS       = cursorhandler.h \
                cursorplugin.h
SOURCES       = cursorhandler.cpp \
                cursorplugin.cpp


DESTDIR      = $(QTDIR)/plugins/imageformats

