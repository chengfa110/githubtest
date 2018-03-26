#-------------------------------------------------
#
# Project created by QtCreator 2018-01-12T17:33:24
#
#-------------------------------------------------

QT       += core gui
QT       += network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = canvas
TEMPLATE = app
CONFIG += c++11


SOURCES += main.cpp\
        mainwindow.cpp \
    piperecevice.cpp \
    canvasview.cpp \
    canvasscene.cpp \
    canvasnode.cpp \
    resizeableitem.cpp \
    canvaspropertybroswer.cpp \
    table.cpp \
    qtpropertybrowser/src/fileedit.cpp \
    qtpropertybrowser/src/fileeditfactory.cpp \
    qtpropertybrowser/src/filepathmanager.cpp \
    qtpropertybrowser/src/qtbuttonpropertybrowser.cpp \
    qtpropertybrowser/src/qteditorfactory.cpp \
    qtpropertybrowser/src/qtgroupboxpropertybrowser.cpp \
    qtpropertybrowser/src/qtpropertybrowser.cpp \
    qtpropertybrowser/src/qtpropertybrowserutils.cpp \
    qtpropertybrowser/src/qtpropertymanager.cpp \
    qtpropertybrowser/src/qttreepropertybrowser.cpp \
    qtpropertybrowser/src/qtvariantproperty.cpp \
    qtpropertybrowser/src/variantfactory.cpp \
    qtpropertybrowser/src/variantmanager.cpp \
    routewindow.cpp \
    routingscene.cpp \
    canvasnodeinfo.cpp \
    dashed.cpp \
    lineitem.cpp \
    modifyroutewidget.cpp

HEADERS  += mainwindow.h \
    piperecevice.h \
    canvasnode.h \
    canvasview.h \
    canvasscene.h \
    canvasconstants.h \
    resizeableitem.h \
    common.h \
    canvaspropertybroswer.h \
    table.h \
    qtpropertybrowser/src/fileedit.h \
    qtpropertybrowser/src/fileeditfactory.h \
    qtpropertybrowser/src/filepathmanager.h \
    qtpropertybrowser/src/qtbuttonpropertybrowser.h \
    qtpropertybrowser/src/qteditorfactory.h \
    qtpropertybrowser/src/qtgroupboxpropertybrowser.h \
    qtpropertybrowser/src/qtpropertybrowser.h \
    qtpropertybrowser/src/qtpropertybrowserutils_p.h \
    qtpropertybrowser/src/qtpropertymanager.h \
    qtpropertybrowser/src/qttreepropertybrowser.h \
    qtpropertybrowser/src/qtvariantproperty.h \
    qtpropertybrowser/src/variantfactory.h \
    qtpropertybrowser/src/variantmanager.h \
    routewindow.h \
    routingscene.h \
    canvasnodeinfo.h \
    dashed.h \
    lineitem.h \
    modifyroutewidget.h

FORMS    += mainwindow.ui

INCLUDEPATH += qtpropertybrowser/src

RESOURCES += \
    qtpropertybrowser/src/qtpropertybrowser.qrc \
    canvasresource.qrc

OTHER_FILES += \
    qtpropertybrowser/src/qtpropertybrowser.pri

macx {
 CONFIG -= app_bundle
 QMAKESPEC = macx-g++
}
