#ifndef EDITOR_H
#define EDITOR_H

#include <QDockWidget>
#include <QMainWindow>

#include <QGroupBox>
#include <QToolButton>

#include <QParallelAnimationGroup>

#include "mainwindow.h"


class Module;


class Editor : public QWidget
{
    Q_OBJECT
public:
    explicit Editor(Module *parent = nullptr, MainWindow *mainWindow = nullptr);

public:
    QWidget* getViewport();

    virtual void updateTime(int t);

protected:
    void setupDrawer();

protected:
    Module *module;
    MainWindow *mainWindow;

    QWidget *viewport;

    QGroupBox *drawer;
    QToolButton *drawerButton;
    QParallelAnimationGroup *drawerAnim;

    int time;

};

#endif // EDITOR_H
