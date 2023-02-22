#include "workspace.h"

#include "module.h"

Workspace::Workspace(MainWindow *parent)
    : QMainWindow{parent}
{
    // set mainWindow
    mainWindow = parent;

    // enable dock nesting
    setDockNestingEnabled(true);

    // set Module list
    modules = new QList<Module*>;

    // no central widget
    setCentralWidget(nullptr);
}

void Workspace::addModule(Module *m) {
    modules->append(m);
}

QList<Module*>* Workspace::getModules() {
    return modules;
}
