#ifndef WORKSPACE_H
#define WORKSPACE_H

#include <QMainWindow>
#include "mainwindow.h"

class Workspace : public QMainWindow
{
    Q_OBJECT
public:
    explicit Workspace(MainWindow *parent = nullptr);

private:
    MainWindow *mainWindow;

};

#endif // WORKSPACE_H
