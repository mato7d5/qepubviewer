#-------------------------------------------------
#
# Project created by QtCreator 2015-10-13T00:03:08
#
#-------------------------------------------------

QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets webkitwidgets

TARGET = QEpubViewer
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++14

INCLUDEPATH += /usr/lib
LIBS += -lepub


SOURCES += main.cpp\
        mainwindow.cpp \
    epubexception.cpp \
    epubpage.cpp \
    epubdocument.cpp \
    epubnetworkmanager.cpp \
    epubnetworkreply.cpp \
    aboutdialog.cpp

HEADERS  += mainwindow.h \
    epubexception.h \
    epubpage.h \
    epubdocument.h \
    epubnetworkmanager.h \
    epubnetworkreply.h \
    aboutdialog.h

FORMS    += mainwindow.ui \
    aboutdialog.ui

RESOURCES += \
    resources.qrc
