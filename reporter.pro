#-------------------------------------------------
#
# Project created by QtCreator 2017-11-12T13:13:26
#
#-------------------------------------------------

QT       += core gui
QT       += sql
QT       += widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = reporter
TEMPLATE = app


UI_DIR = $$PWD

QTPLUGIN += QSQLMYSQL

VERSION_MAJOR = 1
VERSION_MINOR = 0
VERSION_BUILD = 0


# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS\
          "VERSION_MAJOR=$$VERSION_MAJOR"\
          "VERSION_MINOR=$$VERSION_MINOR"\
          "VERSION_BUILD=$$VERSION_BUILD"

VERSION = $${VERSION_MAJOR}.$${VERSION_MINOR}.$${VERSION_BUILD}
# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

include(/home/dave/Documents/sielaff/XlsxWriter/src/xlsx/qtxlsx.pri)
include(vendor/vendor.pri)

SOURCES += \
        main.cpp \
        reporter.cpp \
    databaseconnector.cpp \
    sqlquery.cpp \
    sqlstorage.cpp \
    sqlcontrol.cpp \
    database.cpp \
    sqlparameter.cpp \
    setup.cpp \
    scheduling.cpp \
    shiftschedule.cpp \
    dailyschedule.cpp \
    weeklyschedule.cpp \
    monthlyschedule.cpp \
    customschedule.cpp \
    log.cpp \
    export.cpp \
    exportxls.cpp \
    exportcsv.cpp \
    exporthtml.cpp \
    tableinfo.cpp \
    customscheduling.cpp

HEADERS += \
        reporter.h \
    databaseconnector.h \
    sqlquery.h \
    sqlstorage.h \
    sqlcontrol.h \
    database.h \
    sqlparameter.h \
    setup.h \
    scheduling.h \
    shiftschedule.h \
    dailyschedule.h \
    weeklyschedule.h \
    monthlyschedule.h \
    customschedule.h \
    log.h \
    export.h \
    exportxls.h \
    exportcsv.h \
    exporthtml.h \
    tableinfo.h \
    customscheduling.h

FORMS += \
        reporter.ui \
    tableinfo.ui \
    customscheduling.ui
