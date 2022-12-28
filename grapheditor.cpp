#include "grapheditor.h"

#include <QLabel>

#include "module.h"


GraphEditor::GraphEditor(Module *parent, MainWindow *mainWindow)
    : Editor{parent}
{
    // set parent
    module = parent;

    // set mainWindow
    this->mainWindow = mainWindow;

    // create QLabel as viewport
    QLabel *label = new QLabel(QString("Graph"), module);
    label->setAlignment(Qt::AlignCenter);

    // set viewport
    viewport = label;

    // add viewport to container
    container->addWidget(viewport);

    // set drawer up
    setupDrawer();
}
