#ifndef WORKSPACE_H
#define WORKSPACE_H

#include <QMainWindow>

class MainWindow;
class Module;

class Workspace : public QMainWindow
{
    Q_OBJECT
public:
    explicit Workspace(MainWindow *parent = nullptr);

public:
    void addModule(Module *m);
    QList<Module*>* getModules();

private:
    MainWindow *mainWindow;

    QList<Module*> *modules;

};

#endif // WORKSPACE_H
