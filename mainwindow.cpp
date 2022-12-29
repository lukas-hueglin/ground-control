#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "workspace.h"
#include "module.h"

#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // create new QTabWidget and set as central widget
    tabWidget = new QTabWidget;
    setCentralWidget(tabWidget);

    // add tabs
    tabWidget->addTab(new Workspace(this), QString("Workspace 1"));
    tabWidget->addTab(new Workspace(this), QString("Workspace 2"));

    // create new dataFrame
    m_dataFrame = new DataFrame();

    // hide statusbar
    statusBar()->hide();

    // set title
    setWindowTitle(QString("Ground Control"));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionNew_Editor_triggered()
{
    // get active workspace
    Workspace *workspace = qobject_cast<Workspace*>(tabWidget->currentWidget());

    // create QDockWidget and add it to the workspace
    Module *module = new Module(m_dataFrame, workspace);
    //module->show();
    workspace->addDockWidget(Qt::RightDockWidgetArea, module);
    workspace->addModule(module);
}
