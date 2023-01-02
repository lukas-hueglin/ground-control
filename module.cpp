#include "module.h"

#include <QHBoxLayout>
#include <QComboBox>
#include <QPushButton>
#include <QStatusBar>
#include <QTimer>

#include "grapheditor.h"
#include "inputeditor.h"
#include "timelineeditor.h"
#include "dashboardeditor.h"
#include "globeeditor.h"


Module::Module(DataFrame *p_dataFrame, Workspace *parent)
    : QDockWidget{parent}
{
    // set dataFrame
    m_dataFrame = p_dataFrame;

    // set parent
    m_workspace = parent;

    // create new empty Editor
    m_editor = new Editor(m_dataFrame, this);

    // create the titlebar
    QWidget *titlebar = new QWidget(this);
    setTitleBarWidget(titlebar);

    // create a QHBoxLayout
    QHBoxLayout *titleLayout = new QHBoxLayout;
    titlebar->setLayout(titleLayout);

    // add a QComboBox
    QComboBox *comboBox = new QComboBox(titlebar);
    comboBox->addItem(QString("Empty"));
    comboBox->addItem(QString("Graph Editor"));
    comboBox->addItem(QString("Input Editor"));
    comboBox->addItem(QString("Timeline Editor"));
    comboBox->addItem(QString("Dashboard Editor"));
    comboBox->addItem(QString("Globe Editor"));

    titleLayout->addWidget(comboBox);

    // connect comboBox to slots
    connect(comboBox, &QComboBox::currentIndexChanged, this, &Module::changeEditor);

    // add a QPushButton for splitting the module horizontally
    QPushButton *splitButtonH = new QPushButton(QString("H"), titlebar);
    titleLayout->addWidget(splitButtonH);
    connect(splitButtonH, &QPushButton::pressed, this, &Module::splitModuleHorizontally);

    // add a QPushButton for splitting the module vertically
    QPushButton *splitButtonV = new QPushButton(QString("V"), titlebar);
    titleLayout->addWidget(splitButtonV);
    connect(splitButtonV, &QPushButton::pressed, this, &Module::splitModuleVertically);

    // add a QPushButton for making the module float
    QPushButton *floatButton = new QPushButton(QString("Float"), titlebar);
    titleLayout->addWidget(floatButton);
    connect(floatButton, &QPushButton::pressed, this, &Module::makeFloating);

    // add a QPushButton for closing the module
    QPushButton *closeButton = new QPushButton(QString("X"), titlebar);
    titleLayout->addWidget(closeButton);
    connect(closeButton, &QPushButton::pressed, this, &Module::close);

    // create QMainWindow and set as widget
    m_moduleWindow = new QMainWindow;
    setWidget(m_moduleWindow);
    m_moduleWindow->setCentralWidget(m_editor);

    // set statusbar status
    setStatusOK();
}


// change editor accordingly to QComboBox
void Module::changeEditor(const int index) {
    switch(index) {
        case 0:
            m_editor = new Editor(m_dataFrame, this);
            m_moduleWindow->setCentralWidget(m_editor);
            break;
        case 1:
            m_editor = new GraphEditor(m_dataFrame, this);
            m_moduleWindow->setCentralWidget(m_editor);
            break;
        case 2:
            m_editor = new InputEditor(m_dataFrame, this);
            m_moduleWindow->setCentralWidget(m_editor);
            break;
        case 3:
            m_editor = new TimelineEditor(m_dataFrame, this);
            m_moduleWindow->setCentralWidget(m_editor);
            break;
        case 4:
            m_editor = new DashboardEditor(m_dataFrame, this);
            m_moduleWindow->setCentralWidget(m_editor);
            break;
        case 5:
            m_editor = new GlobeEditor(m_dataFrame, this);
            m_moduleWindow->setCentralWidget(m_editor);
            break;
    }
}


// splits the module into two modules, these are horizontally arranged
void Module::splitModuleHorizontally() {
    // let the parent workspace split the Modules
    if (m_workspace != nullptr){
        Module* newModule = new Module(m_dataFrame, m_workspace);
        m_workspace->splitDockWidget(this, newModule, Qt::Horizontal);
        m_workspace->addModule(newModule);
    } else {

    }
}

// splits the module into two modules, these are vertically arranged
void Module::splitModuleVertically() {
    // let the parent workspace split the Modules
    if (m_workspace != nullptr){
        Module* newModule = new Module(m_dataFrame, m_workspace);
        m_workspace->splitDockWidget(this, newModule, Qt::Vertical);
        m_workspace->addModule(newModule);
    } else {

    }
}


// makes the module float
void Module::makeFloating() {
    setFloating(true);
}


// change colors of statusbar
void Module::setStatusSuccess(QString message) {
    m_moduleWindow->statusBar()->setStyleSheet(QString("background-color: rgb(52, 186, 45);"));
    m_moduleWindow->statusBar()->showMessage(QString(QChar(0x2713)) + QString("   ") + message);

    // the success status will only be shown shortly
    QTimer::singleShot(7000, [this](){setStatusOK(QString(""));});
}

void Module::setStatusWorking(QString message) {
    m_moduleWindow->statusBar()->setStyleSheet(QString("background-color: rgb(40, 147, 247);"));
    m_moduleWindow->statusBar()->showMessage(QString(QChar(0x2B6F)) + QString("   ") + message);
}

void Module::setStatusOK(QString message) {
    m_moduleWindow->statusBar()->setStyleSheet(QString("background-color: rgb(152, 14, 227);"));
    m_moduleWindow->statusBar()->showMessage(QString(QChar(0x276E)) + QString(QChar(0x002F)) + QString(QChar(0x276F)) + QString("   ") + message);
}

void Module::setStatusFail(QString message) {
    m_moduleWindow->statusBar()->setStyleSheet(QString("background-color: rgb(201, 26, 26);"));
    m_moduleWindow->statusBar()->showMessage(QString(QChar(0x2A2F)) + QString("   ") + message);
}


Editor* Module::getEditor(){
    return m_editor;
}
