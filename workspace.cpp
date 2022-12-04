#include "workspace.h"

Workspace::Workspace(QWidget *parent)
    : QMainWindow{parent}
{
    // enable dock nesting
    setDockNestingEnabled(true);
}
