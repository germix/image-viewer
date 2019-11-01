#-------------------------------------------------
#
# Project created by QtCreator 2017-11-22T14:28:42
#
#-------------------------------------------------

QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

win32
{
    QT += winextras
}

#DEFINES += USE_CONFIG

#---------------------------------------------------------------------------------------------------
# Versión
#---------------------------------------------------------------------------------------------------
DEFINES += MAJOR_VERSION=1
DEFINES += MINOR_VERSION=0

#---------------------------------------------------------------------------------------------------
# Target
#---------------------------------------------------------------------------------------------------
CONFIG(debug, debug|release) {
TARGET = ImageViewer_d
DEFINES += DEBUG
} else {
TARGET = ImageViewer
}

#---------------------------------------------------------------------------------------------------
# Directorio de destino
#---------------------------------------------------------------------------------------------------
DESTDIR = ../bin

#---------------------------------------------------------------------------------------------------
# Archivos de código
#---------------------------------------------------------------------------------------------------

SOURCES += \
    src/main.cpp \
    src/AboutDialog.cpp \
    src/MainWindow.cpp \
    src/PropertiesDialog.cpp \
    src/PropertiesFormGeneral.cpp \
    src/utils/ShowInContainerFolder.cpp \
    src/utils/IconFromFile.cpp \
    src/PropertiesFormImage.cpp \
    src/utils/SetWallpaper.cpp \
    src/Languages.cpp \
    src/SettingsDialog.cpp \
    src/utils/AssociateExtensions.cpp \
    src/ImageViewer.cpp

HEADERS += \
    src/AboutDialog.h \
    src/MainWindow.h \
    src/Settings.h \
    src/PropertiesDialog.h \
    src/PropertiesFormGeneral.h \
    src/utils/shell.h \
    src/PropertiesFormImage.h \
    src/Languages.h \
    src/SettingsDialog.h \
    src/ImageViewer.h

FORMS += \
    src/AboutDialog.ui \
    src/MainWindow.ui \
    src/PropertiesDialog.ui \
    src/PropertiesFormGeneral.ui \
    src/PropertiesFormImage.ui \
    src/SettingsDialog.ui

#---------------------------------------------------------------------------------------------------
# Archivos de recursos
#---------------------------------------------------------------------------------------------------

RESOURCES += res/resource.qrc

win32:RC_FILE = res/resource_win32.rc

#---------------------------------------------------------------------------------------------------
# Archivos de traducción
#---------------------------------------------------------------------------------------------------
TRANSLATIONS = \
    translate/imageviewer_es.ts \
    translate/imageviewer_en.ts \
    translate/imageviewer_it.ts
