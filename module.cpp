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


Module::Module(Workspace *parent, MainWindow *mainWindow)
    : QDockWidget{parent}
{
    // set parent
    workspace = parent;

    // set mainWindow
    this->mainWindow = mainWindow;

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
    comboBox->addItem(QString("Input Editor"));
    comboBox->addItem(QString("Timeline Editor"));
    comboBox->addItem(QString("Dashboard Editor"));

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
    moduleWindow = new QMainWindow;
    setWidget(moduleWindow);
    moduleWindow->setCentralWidget(editor);

    // set statusbar status
    setStatusOK();
}


// change editor accordingly to QComboBox
void Module::changeEditor(const int index) {
    switch(index) {
        case 0:
            editor = new Editor(this, mainWindow);
            moduleWindow->setCentralWidget(editor);
            break;
        case 1:
            editor = new GraphEditor(this, mainWindow);
            moduleWindow->setCentralWidget(editor);
            break;
        case 2:
            editor = new InputEditor(this, mainWindow);
            moduleWindow->setCentralWidget(editor);
            break;
        case 3:
            editor = new TimelineEditor(this, mainWindow);
            moduleWindow->setCentralWidget(editor);
            break;
        case 4:
            editor = new DashboardEditor(this, mainWindow);
            moduleWindow->setCentralWidget(editor);
            break;
    }
}


// splits the module into two modules, these are horizontally arranged
void Module::splitModuleHorizontally() {
    // let the parent workspace split the Modules
    if (workspace != nullptr){
        Module* newModule = new Module(workspace, mainWindow);
        workspace->splitDockWidget(this, newModule, Qt::Horizontal);
        workspace->addModule(newModule);
    } else {

    }
}

// splits the module into two modules, these are vertically arranged
void Module::splitModuleVertically() {
    // let the parent workspace split the Modules
    if (workspace != nullptr){
        Module* newModule = new Module(workspace, mainWindow);
        workspace->splitDockWidget(this, newModule, Qt::Vertical);
        workspace->addModule(newModule);
    } else {

    }
}


// makes the module float
void Module::makeFloating() {
    setFloating(true);
}


// change colors of statusbar
void Module::setStatusSuccess(QString message) {
    moduleWindow->statusBar()->setStyleSheet(QString("background-color: rgb(52, 186, 45);"));
    moduleWindow->statusBar()->showMessage(QString(QChar(0x2713)) + QString("   ") + message);

    // the success status will only be shown shortly
    QTimer *timer = new QTimer(this);

    // connect timeout
    connect(timer, &QTimer::timeout, [this, message](){setStatusOK((message));});

    timer->start(7000);
}

void Module::setStatusWorking(QString message) {
    moduleWindow->statusBar()->setStyleSheet(QString("background-color: rgb(40, 147, 247);"));
    moduleWindow->statusBar()->showMessage(QString(QChar(0x2B6F)) + QString("   ") + message);
}

void Module::setStatusOK(QString message) {
    moduleWindow->statusBar()->setStyleSheet(QString("background-color: rgb(152, 14, 227);"));
    moduleWindow->statusBar()->showMessage(QString(QChar(0x276E)) + QString(QChar(0x002F)) + QString(QChar(0x276F)) + QString("   ") + message);
}

void Module::setStatusFail(QString message) {
    moduleWindow->statusBar()->setStyleSheet(QString("background-color: rgb(201, 26, 26);"));
    moduleWindow->statusBar()->showMessage(QString(QChar(0x2A2F)) + QString("   ") + message);
}


Editor* Module::getEditor(){
    return editor;
}
