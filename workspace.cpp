#include "workspace.h"

Workspace::Workspace(QWidget *parent)
    : QMainWindow{parent}
{
    // set parent
    this->parent = qobject_cast<QMainWindow*>(parent);

    // enable dock nesting
    setDockNestingEnabled(true);
}
