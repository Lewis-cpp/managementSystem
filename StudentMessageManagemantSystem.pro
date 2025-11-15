
QT += core gui widgets sql
QT += core gui widgets sql concurrent
TARGET = StudentMessageManagementSystem
TEMPLATE = app
CONFIG += c++17

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    StudentMessageManagementSystem.cpp

HEADERS += \
    mainwindow.h \
    student.h \
    binarysearchtree.h \
    StudentMessageManagementSystem.h

FORMS += \
    StudentMessageManagementSystem.ui

