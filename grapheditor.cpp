#include "grapheditor.h"

#include <QLabel>

GraphEditor::GraphEditor(QWidget *parent)
    : Editor{parent}
{
    // create QLabel as viewport
    QLabel *label = new QLabel(QString("Graph"), parent);
    label->setAlignment(Qt::AlignCenter);

    // set viewport
    viewport = label;

    // set drawer up
    setupDrawer();
}
