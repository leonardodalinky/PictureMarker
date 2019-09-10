#-------------------------------------------------
#
# Project created by QtCreator 2019-08-25T21:09:22
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PictureMarker
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

CONFIG += c++11

SOURCES += \
        file.cpp \
        filemanager.cpp \
        graphicscircleitem.cpp \
        graphicsitem.cpp \
        graphicspdrawingpaditem.cpp \
        graphicsrectitem.cpp \
        historymanager.cpp \
        main.cpp \
        mainwindow.cpp \
        marker.cpp \
        marker3d.cpp \
        marker3ddrawingpad.cpp \
        marker3drect.cpp \
        markercircle.cpp \
        markerdrawingpad.cpp \
        markermanager.cpp \
        markerrect.cpp \
        pixelpointpad.cpp \
        savereminddialog.cpp \
        toolmanager.cpp \
        view2d.cpp \
        view3d.cpp

HEADERS += \
        file.h \
        filemanager.h \
        graphicscircleitem.h \
        graphicsitem.h \
        graphicspdrawingpaditem.h \
        graphicsrectitem.h \
        historymanager.h \
        mainwindow.h \
        marker.h \
        marker3d.h \
        marker3ddrawingpad.h \
        marker3drect.h \
        markercircle.h \
        markerdrawingpad.h \
        markermanager.h \
        markerrect.h \
        pixelpointpad.h \
        savereminddialog.h \
        toolmanager.h \
        view2d.h \
        view3d.h

FORMS += \
        filemanager.ui \
        mainwindow.ui \
        markermanager.ui \
        savereminddialog.ui \
        toolmanager.ui \
        view2d.ui \
        view3d.ui

#INCLUDEPATH += F:\Program_Files\opencv-3.2.0_Qtbuild\include\
#LIBS += F:\Program_Files\opencv-3.2.0_Qtbuild\lib\libopencv_*.a



# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    rsc.qrc
