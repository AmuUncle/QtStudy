
greaterThan(QT_MAJOR_VERSION, 4): QT += printsupport

INCLUDEPATH += $$PWD

HEADERS += \
    $$PWD/qcustomplot.h \
    $$PWD/qcustomplottest.h

SOURCES += \
    $$PWD/qcustomplot.cpp \
    $$PWD/qcustomplottest.cpp
