QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    dashboardeditor.cpp \
    dataframe.cpp \
    editor.cpp \
    grapheditor.cpp \
    inputeditor.cpp \
    main.cpp \
    mainwindow.cpp \
    module.cpp \
    timelineeditor.cpp \
    viewport.cpp \
    workspace.cpp

HEADERS += \
    dashboardeditor.h \
    dataframe.h \
    editor.h \
    grapheditor.h \
    inputeditor.h \
    mainwindow.h \
    module.h \
    timelineeditor.h \
    viewport.h \
    workspace.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
