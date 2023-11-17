#include "helpers/titlebar.h"

#include <QPushButton>

#include <module.h>

Titlebar::Titlebar(char editorIndex, Module *parent)
    : QWidget{parent}
{
    // create a QHBoxLayout
    QHBoxLayout *titleLayout = new QHBoxLayout;
    titleLayout->setContentsMargins(7, 7, 7, 7);
    setLayout(titleLayout);

    // add a QComboBox
    m_comboBox = new QComboBox(this);
    m_comboBox->setMaximumWidth(200);

    m_comboBox->addItem(QIcon(":/icons/dark_empty.svg"), QString("Empty"));
    m_comboBox->addItem(QIcon(":/icons/dark_graph.svg"), QString("Graph Editor"));
    m_comboBox->addItem(QIcon(":/icons/dark_input.svg"), QString("IO Editor"));
    m_comboBox->addItem(QIcon(":/icons/dark_timeline.svg"), QString("Timeline Editor"));
    m_comboBox->addItem(QIcon(":/icons/dark_dashboard.svg"), QString("Dashboard Editor"));
    m_comboBox->addItem(QIcon(":/icons/dark_globe.svg"), QString("Globe Editor"));
    m_comboBox->addItem(QIcon(":/icons/dark_map.svg"), QString("Map Editor"));
    m_comboBox->addItem(QIcon(":/icons/dark_artificial_horizon.svg"), QString("Artificial Horizon"));

    m_comboBox->setCurrentIndex(editorIndex);

    titleLayout->addWidget(m_comboBox);
    titleLayout->addStretch();

    // connect comboBox to slots
    connect(m_comboBox, &QComboBox::currentIndexChanged, parent, &Module::changeEditor);

    // add a QPushButton for splitting the module horizontally
    QPushButton *splitButtonH = new QPushButton("", this);
    titleLayout->addWidget(splitButtonH);
    splitButtonH->setProperty("cssClass", "splitH");
    connect(splitButtonH, &QPushButton::pressed, parent, &Module::splitModuleHorizontally);

    // add a QPushButton for splitting the module vertically
    QPushButton *splitButtonV = new QPushButton("", this);
    titleLayout->addWidget(splitButtonV);
    splitButtonV->setProperty("cssClass", "splitV");
    connect(splitButtonV, &QPushButton::pressed, parent, &Module::splitModuleVertically);

    // add a QPushButton for making the module float
    QPushButton *floatButton = new QPushButton("", this);
    titleLayout->addWidget(floatButton);
    floatButton->setProperty("cssClass", "float");
    connect(floatButton, &QPushButton::pressed, parent, &Module::makeFloating);

    // add a QPushButton for closing the module
    QPushButton *closeButton = new QPushButton("", this);
    titleLayout->addWidget(closeButton);
    closeButton->setProperty("cssClass", "close");
    connect(closeButton, &QPushButton::pressed, parent, &Module::close);

    // add property
    setProperty("cssClass", "titlebar");
}

QSize Titlebar::sizeHint() const {
    return QSize(300, 35);
}

QSize Titlebar::minimumSizeHint() const {
    return QSize(0, 0);
}

QComboBox* Titlebar::getComboBox() {
    return m_comboBox;
}
