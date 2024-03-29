QT       += core gui charts widgets openglwidgets xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

CONFIG += c++17

ARCGIS_RUNTIME_VERSION = 200.1.0
include($$PWD/arcgis/arcgisruntime.pri)

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += \
    $$PWD/include

SOURCES += \
    $$PWD/src/editors/aheditor.cpp \
    $$PWD/src/helpers/artificialhorizon.cpp \
    $$PWD/src/editors/dashboardeditor.cpp \
    $$PWD/src/dataframe.cpp \
    $$PWD/src/helpers/dataseries.cpp \
    $$PWD/src/editor.cpp \
    $$PWD/src/editors/globeeditor.cpp \
    $$PWD/src/editors/grapheditor.cpp \
    $$PWD/src/editors/inputeditor.cpp \
    $$PWD/src/main.cpp \
    $$PWD/src/mainwindow.cpp \
    $$PWD/src/editors/mapeditor.cpp \
    $$PWD/src/module.cpp \
    $$PWD/src/editors/timelineeditor.cpp \
    $$PWD/src/viewport.cpp \
    $$PWD/src/workspace.cpp \
    $$PWD/src/helpers/qcustomplot.cpp

HEADERS += \
    $$PWD/include/editors/aheditor.h \
    $$PWD/include/helpers/artificialhorizon.h \
    $$PWD/include/editors/dashboardeditor.h \
    $$PWD/include/dataframe.h \
    $$PWD/include/helpers/dataseries.h \
    $$PWD/include/editor.h \
    $$PWD/include/editors/globeeditor.h \
    $$PWD/include/editors/grapheditor.h \
    $$PWD/include/editors/inputeditor.h \
    $$PWD/include/mainwindow.h \
    $$PWD/include/editors/mapeditor.h \
    $$PWD/include/module.h \
    $$PWD/include/editors/timelineeditor.h \
    $$PWD/include/viewport.h \
    $$PWD/include/workspace.h \
    $$PWD/include/helpers/qcustomplot.h

FORMS += \
    $$PWD/forms/mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    $$PWD/style/colorpresets.xml \
    $$PWD/style/darktheme.xml \
    $$PWD/style/default.xml \
    $$PWD/arcgis/key.txt \
    $$PWD/style/stylesheet.qss

RESOURCES += \
    $$PWD/resources.qrc
