
TARGET  = q_kap
VERSION = 1.0.0

TEMPLATE     = lib
CONFIG      += plugin

HEADERS += bsb.h kap_plugin.h kap_handler.h
SOURCES += bsb_io.cpp kap_plugin.cpp kap_handler.cpp


DESTDIR      = $(QTDIR)/plugins/imageformats


