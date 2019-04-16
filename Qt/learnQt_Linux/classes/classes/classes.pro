TEMPLATE = app
TARGET = Cracker_Boi_2

QT = core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
SOURCES += main.cpp \
    window.cpp

HEADERS += \
    window.h
