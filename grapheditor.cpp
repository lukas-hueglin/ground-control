#include "grapheditor.h"

#include <QLabel>


GraphEditor::GraphEditor(DataFrame *p_dataFrame, QWidget *parent)
    : Editor{p_dataFrame, parent}
{
    // create QLabel as viewport
    QLabel *label = new QLabel(QString("Graph"), parent);
    label->setAlignment(Qt::AlignCenter);

    // set viewport
    m_viewport = label;

    // add viewport to container
    m_container->addWidget(m_viewport);

    // set drawer up
    setupDrawer();
}
