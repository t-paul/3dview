# This file is generated automatically. Do not edit.
# Use project properties -> Build -> Qt -> Expert -> Custom Definitions.
TEMPLATE = app
DESTDIR = dist/Debug/GNU-Linux-x86
TARGET = 3dview
VERSION = 1.0.0
CONFIG -= debug_and_release app_bundle lib_bundle
CONFIG += link_pkgconfig debug 
PKGCONFIG += glew assimp
QT = core gui widgets opengl
SOURCES += src/Mesh.cpp src/Transform.cpp src/main.cpp ui/GLWidget.cpp ui/MainWindow.cpp
HEADERS += src/Mesh.h src/Transform.h ui/GLWidget.h ui/MainWindow.h
FORMS += ui/GLWidget.ui ui/MainWindow.ui
RESOURCES +=
TRANSLATIONS +=
OBJECTS_DIR = build/Debug/GNU-Linux-x86
MOC_DIR = 
RCC_DIR = 
UI_DIR = ui
QMAKE_CC = gcc
QMAKE_CXX = g++
DEFINES += 
INCLUDEPATH += src 
LIBS += 
