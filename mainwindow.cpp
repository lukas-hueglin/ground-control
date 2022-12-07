#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "workspace.h"
#include "module.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // create new QTabWidget and set as central widget
    tabWidget = new QTabWidget;
    setCentralWidget(tabWidget);

    // add tabs
    tabWidget->addTab(new Workspace(this), QString("Workspace 1"));
    tabWidget->addTab(new Workspace(this), QString("Workspace 2"));

    // hide statusbar
    statusBar()->hide();

    // set title
    setWindowTitle(QString("Ground Control"));

    // create QLists
    times = new QList<QDateTime*>;
    values = new QList<QMap<QString, float>*>;
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::setFile(QFile *f){
    file = f;
}


void MainWindow::on_actionNew_Editor_triggered()
{
    // get active workspace
    Workspace *workspace = qobject_cast<Workspace*>(tabWidget->currentWidget());

    // create QDockWidget and add it to the workspace
    Module *module = new Module(workspace, this);
    //module->show();
    workspace->addDockWidget(Qt::RightDockWidgetArea, module);
}

void MainWindow::loadDataFromFile() {
    // check if file exists
    if (file != nullptr) {
        // check if file can be opened
        if (!file->open(QIODevice::ReadOnly)){
            return;
        }
        // read first line and get headers
        QList<QString> headers;
        QString firstLine = file->readLine();
        firstLine = firstLine.left(firstLine.size()-2);

        for (QString str : firstLine.split(",")){
            headers.append(str);
        }

        // load all lines
        while (!file->atEnd()) {
            // read line
            QString line = file->readLine();
            line = line.left(line.size()-2); // remove \r\n

            // store single strings in QList
            QList<QString> elements = line.split(",").toList();

            // create QMap
            QMap<QString, float> *map = new QMap<QString, float>;

            // for every col
            for (int i = 0; i < headers.size(); ++i){
                // if col is time
                if (headers.at(i) == QString("time")) {
                    QString date = elements.at(i).split(" ").at(0);
                    QString time = elements.at(i).split(" ").at(1);

                    int y = date.split("-").at(0).toInt();
                    int mo = date.split("-").at(1).toInt();
                    int d = date.split("-").at(2).toInt();

                    int h = time.split(":").at(0).toInt();
                    int mi = time.split(":").at(1).toInt();
                    int s = time.split(":").at(2).toInt();

                    QDateTime *datetime = new QDateTime(QDate(y, mo, d), QTime(h, mi, s));
                    times->append(datetime);
                }
                // else if it is not time
                else{
                    map->insert(headers.at(i), elements.at(i).toFloat());
                }
            }

            values->append(map);
        }
    }
}

int MainWindow::getDataLen() {
    return times->size();
}

