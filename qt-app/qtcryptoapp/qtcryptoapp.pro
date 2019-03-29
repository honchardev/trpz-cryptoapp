#-------------------------------------------------
#
# Project created by QtCreator 2019-03-13T20:33:38
#
#-------------------------------------------------

QT       += core gui sql network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = qtcryptoapp
TEMPLATE = app

QMAKE_LFLAGS += -no-pie

# Search for qcustomplot shared libraries in executable directory, not in gcc/lib or /usr/lib
QMAKE_LFLAGS += -Wl,-rpath,"'$$ORIGIN'"

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# Tell the qcustomplot header that it will be used as library:
DEFINES += QCUSTOMPLOT_USE_LIBRARY

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

# Link with debug version of qcustomplot if compiling in debug mode, else with release library:
CONFIG(debug, release|debug) {
  win32:QCPLIB = qcustomplotdwin
  else: QCPLIB = qcustomplotd
} else {
  win32:QCPLIB = qcustomplotwin
  else: QCPLIB = qcustomplot
}
LIBS += -L./ -l$$QCPLIB

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    dbwrapper.cpp \
    exchange.cpp \
    cmcscrapper.cpp \
    chart.cpp

HEADERS += \
        mainwindow.h \
    dbwrapper.h \
    exchange.h \
    cmcscrapper.h \
    chart.h

FORMS += \
        mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
