#-------------------------------------------------
#
# Project created by QtCreator 2018-03-28T15:25:17
#
#-------------------------------------------------

QT  += core gui
QT  += charts
QT  += datavisualization


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LDC_Search
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


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    search.cpp \
    statistics.cpp \
    distortion.cpp

HEADERS += \
        mainwindow.h \
    search.h \
    statistics.h \
    distortion.h

FORMS += \
        mainwindow.ui
