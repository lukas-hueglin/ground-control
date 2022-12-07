#ifndef MODULE_H
#define MODULE_H

#include <QMainWindow>
#include <QDockWidget>

#include "editor.h"
#include "workspace.h"
#include "mainwindow.h"

class Module : public QDockWidget
{
    Q_OBJECT
public:
    explicit Module(Workspace *parent = nullptr, MainWindow *mainWindow = nullptr);


public:
    void setStatusSuccess(QString message = QString());
    void setStatusWorking(QString message = QString());
    void setStatusOK(QString message = QString());
    void setStatusFail(QString message = QString());

public slots:
    void changeEditor(const int index);
    void splitModuleHorizontally();
    void splitModuleVertically();
    void makeFloating();

private:
    Workspace *workspace;
    MainWindow *mainWindow;

    QMainWindow *moduleWindow;

    Editor *editor;
};

#endif // MODULE_H
