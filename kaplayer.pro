QT += core gui widgets multimedia multimediawidgets opengl
SOURCES += main.cpp \
           mainwindow.cpp
HEADERS += mainwindow.h
FORMS += mainwindow.ui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17