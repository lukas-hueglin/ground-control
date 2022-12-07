#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "workspace.h"
#include "module.h"
#include "editor.h"

#include <QTimer>

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

    // set dataLen
    dataLen = 0;

    // set time;
    time = 0;

    // set play
    play = false;
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
    workspace->addModule(module);
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

        dataLen = times->size();
    }
}

int MainWindow::getDataLen() {
    return dataLen;
}

void MainWindow::updateTime(int t) {
    time = t;

    // get active workspace
    Workspace *workspace = qobject_cast<Workspace*>(tabWidget->currentWidget());

    QList<Module*>* modules = workspace->getModules();

    for (Module* m : *modules) {
        m->getEditor()->updateTime(time);
    }
}


QDateTime* MainWindow::getDateTime(int i){
    if (!times->empty()){
        return times->at(i);
    }
    return nullptr;
}

float MainWindow::getValue(int i, QString key){
    if (!values->empty()) {
        return values->at(i)->value(key);
    }
    return 0;
}


void MainWindow::playTime(bool start){
    if (play || start){
        QDateTime *t1 = getDateTime(time);
        QDateTime *t2 = getDateTime(time+1);

        int ms = t1->daysTo(*t2)*1000*60*60*24 + t1->time().msecsTo(t2->time());

        // update time
        updateTime(time+1);

        // set play
        play = true;

        // create Timer
        QTimer::singleShot(ms, [this](){playTime(false);});
    }
}

void MainWindow::pauseTime() {
    play = false;
}
