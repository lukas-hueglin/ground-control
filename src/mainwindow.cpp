#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "module.h"
#include "workspace.h"

#include <QTimer>
#include <QSizePolicy>

#include <QFile>
#include <QFontDatabase>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // create new QTabWidget and set as central widget
    m_tabWidget = new QTabWidget;
    setCentralWidget(m_tabWidget);

    // create new dataFrame
    m_dataFrame = new DataFrame();

    // hide statusbar
    statusBar()->hide();

    // set title
    setWindowTitle(QString("Ground Control"));

    // set icon
    setWindowIcon(QIcon(":/icons/GroundControl.svg"));

    // set layout
    QDomDocument *xml = new QDomDocument;
    QFile layoutFile(":/layouts/default.xml");

    if (layoutFile.open(QIODevice::ReadOnly)) {
        xml->setContent(&layoutFile);
        parseLayout(xml);
    }


    // load roboto font
    QStringList weights = {"Black", "BlackItalic", "Bold", "BoldItalic", "Italic", "Light", "LightItalic", "Medium", "MediumItalic", "Regular", "Thin", "ThinItalic"};

    for (QString w : weights) {
        QFontDatabase::addApplicationFont(":/fonts/Roboto-"+w+".ttf");
    }

    // set theme
    QFile styleFile(":/themes/stylesheet.css");
    QFile themeFile(":/themes/darktheme.xml");

    if (styleFile.open(QFile::ReadOnly | QIODevice::Text) && themeFile.open(QIODevice::ReadOnly)) {
        QString str = styleFile.readAll();
        xml->setContent(&themeFile);

        parseStylesheet(xml, str);
    }
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::parseLayout(QDomDocument *xml) {
    QDomElement layout = xml->documentElement();

    // go through all workspaces
    QDomElement element = layout.firstChildElement();

    for(int i = 0; !element.isNull(); ++i) {
        QString title = element.attribute("TITLE", "NoTitle");
        QString icon = element.attribute("ICON", "NoIcon");

        // assert if no title nor icon is given
        assert(title != "NoTitle");
        assert(icon != "NoIcon");

        // create workspace
        Workspace *workspace = new Workspace;
        m_tabWidget->addTab(workspace, QIcon(icon), title);

        // create new module
        Module *m = new Module(m_dataFrame, workspace);
        workspace->addDockWidget(Qt::RightDockWidgetArea, m);
        workspace->addModule(m);

        // parse modules
        parseModule(workspace, m, element.firstChildElement());

        // get next workspace
        element = element.nextSiblingElement();
    }
}

void MainWindow::parseModule(Workspace *w, Module *m, QDomElement e) {
    // check which type the element is
    if (e.tagName() == "splitter"){
        float ratio = e.attribute("RATIO", "0.5").toFloat();
        bool horizontal = e.attribute("ORIENTATION", "horizontal") == "vertical";
        Module *sm;

        // check if horizontal and split module
        if(horizontal) {
            sm = m->splitModuleHorizontally();
            w->resizeDocks({m, sm}, {int(100*ratio), int(100*(1-ratio))}, Qt::Horizontal);
        } else {
            sm = m->splitModuleVertically();
            w->resizeDocks({m, sm}, {int(100*ratio), int(100*(1-ratio))}, Qt::Vertical);
        }

        // continue for children
        QDomElement firstChild = e.firstChildElement();
        QDomElement secondChild = firstChild.nextSiblingElement();

        parseModule(w, m, firstChild);
        parseModule(w, sm, secondChild);

    } else {
        int index = e.text().toInt();
        m->changeComboBox(index);
    }
}

void MainWindow::parseStylesheet(QDomDocument *xml, QString &str) {
    QDomElement palette = xml->documentElement();

    // go through all elements
    QDomElement element = palette.firstChildElement();

    while(!element.isNull()) {
        QString name = element.attribute("NAME", "noName");
        QString color = element.text();
        assert(name != "noName");

        str.replace(name, color);
        element = element.nextSiblingElement();
    }

    // set stylesheet
    setStyleSheet(str);
}

void MainWindow::on_actionNew_Editor_triggered() {
    // get active workspace
    Workspace *workspace = qobject_cast<Workspace*>(m_tabWidget->currentWidget());

    // create QDockWidget and add it to the workspace
    Module *module = new Module(m_dataFrame, workspace);
    //module->show();
    workspace->addDockWidget(Qt::RightDockWidgetArea, module);
    workspace->addModule(module);
}
