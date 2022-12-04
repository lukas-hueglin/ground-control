#include "module.h"

#include <QHBoxLayout>
#include <QComboBox>
#include <QPushButton>

#include "grapheditor.h"


Module::Module(QWidget *parent)
    : QDockWidget{parent}
{
    // store parent
    workspace = qobject_cast<QMainWindow*>(parent);

    // create new empty Editor
    editor = new Editor(this);

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
    comboBox->addItem(QString("Editor 2"));
    comboBox->addItem(QString("Editor 3"));

    titleLayout->addWidget(comboBox);

    // connect comboBox to slots
    connect(comboBox, &QComboBox::currentIndexChanged, this, &Module::changeEditor);

    // add a QPushButton for splitting the module horizontally
    QPushButton *splitButtonH = new QPushButton(QString("Split Module H"), titlebar);
    titleLayout->addWidget(splitButtonH);
    connect(splitButtonH, &QPushButton::pressed, this, &Module::splitModuleHorizontally);

    // add a QPushButton for splitting the module vertically
    QPushButton *splitButtonV = new QPushButton(QString("Split Module V"), titlebar);
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
    mainWindow = new QMainWindow;
    setWidget(mainWindow);
    mainWindow->setCentralWidget(editor->getViewport());
}


// change editor accordingly to QComboBox
void Module::changeEditor(const int index) {
    switch(index) {
        case 0:
            editor = new Editor(this);
            mainWindow->setCentralWidget(editor->getViewport());
            break;
        case 1:
            editor = new GraphEditor(this);
            mainWindow->setCentralWidget(editor->getViewport());
            break;
    }
}


// splits the module into two modules, these are horizontally arranged
void Module::splitModuleHorizontally() {
    // let the parent workspace split the Modules
    if (workspace != nullptr){
        workspace->splitDockWidget(this, new Module(workspace), Qt::Horizontal);
    } else {

    }
}

// splits the module into two modules, these are vertically arranged
void Module::splitModuleVertically() {
    // let the parent workspace split the Modules
    if (workspace != nullptr){
        workspace->splitDockWidget(this, new Module(workspace), Qt::Vertical);
    } else {

    }
}


// makes the module float
void Module::makeFloating() {
    setFloating(true);
}

