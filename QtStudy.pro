#-------------------------------------------------
#
# Project created by QtCreator 2022-06-23T11:09:27
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtStudy
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

INCLUDEPATH += $$PWD/include
SOURCES += \
        $$PWD/source/main.cpp \
        $$PWD/source/mainwindow.cpp \
    $$PWD/source/navbutton.cpp

HEADERS += \
        $$PWD/include/mainwindow.h \
        $$PWD/include/navbutton.h \
    include/global.h

FORMS += \
        $$PWD/ui/mainwindow.ui

include($$PWD/BeautifulTable/BeautifulTable.pri)
include($$PWD/Gradient/Gradient.pri)
include($$PWD/ImageConvert/ImageConvert.pri)
include($$PWD/LiquidPlot/LiquidPlot.pri)
include($$PWD/PerfectClock/PerfectClock.pri)
include($$PWD/Animation/Animation.pri)
include($$PWD/Layout/Layout.pri)
include($$PWD/Radar/Radar.pri)
include($$PWD/IconFont/IconFont.pri)
include($$PWD/CarouselPane/CarouselPane.pri)
include($$PWD/ThreadTool/ThreadTool.pri)
include($$PWD/CircularDial/CircularDial.pri)
include($$PWD/LoadingDialog/LoadingDialog.pri)
include($$PWD/FileTransfer/FileTransfer.pri)
include($$PWD/Captcha/Captcha.pri)
include($$PWD/LuckyTurntable/LuckyTurntable.pri)
include($$PWD/QLabelPlus/QLabelPlus.pri)
include($$PWD/TimeLine/TimeLine.pri)
include($$PWD/QCustomPlot/QCustomPlot.pri)
include($$PWD/EPushButton/EPushButton.pri)

RESOURCES += \
    $$PWD/res/qtstudy.qrc
