# This file is generated automatically. Do not edit.
# Use project properties -> Build -> Qt -> Expert -> Custom Definitions.
TEMPLATE = app
DESTDIR = dist/Release/GNU-Linux-x86
TARGET = 3dview
VERSION = 1.0.0
CONFIG -= debug_and_release app_bundle lib_bundle
CONFIG += release 
PKGCONFIG +=
QT = core gui widgets
SOURCES += src/Mesh.cpp src/Transform.cpp src/main.cpp ui/GLWidget.cpp ui/MainWindow.cpp
HEADERS += src/Mesh.h src/Transform.h ui/GLWidget.h ui/MainWindow.h
FORMS += ui/GLWidget.ui ui/MainWindow.ui
RESOURCES += resources/3dview.qrc
TRANSLATIONS +=
OBJECTS_DIR = build/Release/GNU-Linux-x86
MOC_DIR = 
RCC_DIR = 
UI_DIR = 
QMAKE_CC = gcc
QMAKE_CXX = g++
DEFINES += 
INCLUDEPATH += 
LIBS += 
