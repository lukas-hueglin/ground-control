#include "workspace.h"

Workspace::Workspace(MainWindow *parent)
    : QMainWindow{parent}
{
    // set mainWindow
    mainWindow = parent;

    // enable dock nesting
    setDockNestingEnabled(true);
}
