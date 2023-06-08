QT       += core gui charts widgets openglwidgets xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

CONFIG += c++17

ARCGIS_RUNTIME_VERSION = 200.0.0
include($$PWD/arcgisruntime.pri)

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    dashboardeditor.cpp \
    dataframe.cpp \
    dataseries.cpp \
    editor.cpp \
    globeeditor.cpp \
    grapheditor.cpp \
    inputeditor.cpp \
    main.cpp \
    mainwindow.cpp \
    mapeditor.cpp \
    module.cpp \
    timelineeditor.cpp \
    viewport.cpp \
    workspace.cpp \
    qcustomplot.cpp

HEADERS += \
    dashboardeditor.h \
    dataframe.h \
    dataseries.h \
    editor.h \
    globeeditor.h \
    grapheditor.h \
    inputeditor.h \
    mainwindow.h \
    mapeditor.h \
    module.h \
    timelineeditor.h \
    viewport.h \
    workspace.h \
    qcustomplot.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    colorpresets.xml \
    darktheme.xml \
    default.xml \
    key.txt \
    stylesheet.qss

RESOURCES += \
    Resources.qrc
