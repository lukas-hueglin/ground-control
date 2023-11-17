#include "editor.h"

#include <QPropertyAnimation>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>

#include "helpers/statusbar.h"
#include "helpers/drawer.h"


Editor::Editor(DataFrame *p_dataFrame, QWidget *parent)
    : QWidget{parent}
{
    // set dataFrame
    m_dataFrame = p_dataFrame;

    // create QGridLayout
    m_layout = new QGridLayout(this);
    m_layout->setContentsMargins(5, 5, 5, 5);

    // create new Statusbar
    m_statusbar = new Statusbar(this);
    m_layout->addWidget(m_statusbar, 1, 0, 1, 2);

    // create new Drawer
    m_drawer = new Drawer(this);
    m_layout->addWidget(m_drawer, 0, 0, 1, 1);

    // create new viewport
    m_viewport = new QWidget(this);
    m_layout->addWidget(m_viewport, 0, 1, 1, 1);

    // set streches to minimize statusbar and maximize viewport
    m_layout->setRowStretch(0, 1);
    m_layout->setColumnStretch(1, 1);
}

QWidget* Editor::getViewport() {
    return m_viewport;
}

QSize Editor::sizeHint() const {
    return QSize(300, 200);
}

QSize Editor::minimumSizeHint() const {
    return QSize(0, 0);
}

void Editor::setupViewport() {
    m_layout->removeWidget(m_viewport);
    delete m_viewport;
}

void Editor::setupDrawer() {}

