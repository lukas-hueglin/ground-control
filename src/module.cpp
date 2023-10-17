#include "module.h"

#include <QHBoxLayout>
#include <QPushButton>
#include <QStatusBar>
#include <QTimer>

#include "editors/grapheditor.h"
#include "editors/ioeditor.h"
#include "editors/timelineeditor.h"
#include "editors/dashboardeditor.h"
#include "editors/globeeditor.h"
#include "editors/mapeditor.h"
#include "editors/aheditor.h"


Titlebar::Titlebar(Module *parent)
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


Statusbar::Statusbar(Module *parent)
    : QWidget{parent}
{
    QWidget *w = new QWidget(this);

    // create a QHBoxLayout
    QHBoxLayout *layout = new QHBoxLayout;
    layout->setContentsMargins(0, 0, 0, 0);
    setLayout(layout);

    // create icon
    m_icon = new QLabel("");
    m_icon->setPixmap(QIcon(":/icons/dark_OK.svg").pixmap(QSize(20, 20)));
    m_icon->setAlignment(Qt::AlignCenter);
    m_icon->setFixedSize(QSize(50, 25));
    layout->addWidget(m_icon);


    // create message
    m_message = new QLabel("");
    m_message->setAlignment(Qt::AlignVCenter);
    m_message->setFixedHeight(25);
    layout->addWidget(m_message, 1);

    // set stylesheet
    setStyleSheet(QString("background-color: hsv(279, 94%, 89%);"));
}

QSize Statusbar::sizeHint() const {
    return QSize(300, 25);
}

QSize Statusbar::minimumSizeHint() const {
    return QSize(0, 0);
}

void Statusbar::setIcon(QPixmap pixmap) {
    m_icon->setPixmap(pixmap);
}

void Statusbar::setMessage(QString str) {
    m_message->setText(str);
}


ContainerWidget::ContainerWidget(Module *parent) : QWidget{parent} {}

QSize ContainerWidget::sizeHint() const {
    return QSize(300, 200);
}

QSize ContainerWidget::minimumSizeHint() const {
    return QSize(0, 0);
}


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
    m_titlebar = new Titlebar(this);
    setTitleBarWidget(m_titlebar);

    //create the statusbar
    m_statusbar = new Statusbar(this);

    // create QVBoxLayout
    m_layout = new QVBoxLayout;
    m_layout->setContentsMargins(0, 0, 0, 0);

    // create container widget
    ContainerWidget *container = new ContainerWidget(this);
    setWidget(container);
    container->setLayout(m_layout);

    m_layout->addWidget(m_editor);
    m_layout->addWidget(m_statusbar);
}

void Module::changeComboBox(int index){
    m_titlebar->getComboBox()->setCurrentIndex(index);
}

QSize Module::sizeHint() const {
    return QSize(300, 200);
}

QSize Module::minimumSizeHint() const {
    return QSize(0, 0);
}

// change editor accordingly to QComboBox
void Module::changeEditor(const int index) {
    switch(index) {
        case 0:
            m_layout->removeWidget(m_editor);
            delete m_editor;
            m_editor = new Editor(m_dataFrame, this);
            m_layout->insertWidget(0, m_editor, 1);
            break;
        case 1:
            m_layout->removeWidget(m_editor);
            delete m_editor;
            m_editor = new GraphEditor(m_dataFrame, this);
            m_layout->insertWidget(0, m_editor, 1);
            break;
        case 2:
            m_layout->removeWidget(m_editor);
            delete m_editor;
            m_editor = new IOEditor(m_dataFrame, this);
            m_layout->insertWidget(0, m_editor, 1);
            break;
        case 3:
            m_layout->removeWidget(m_editor);
            delete m_editor;
            m_editor = new TimelineEditor(m_dataFrame, this);
            m_layout->insertWidget(0, m_editor, 1);
            break;
        case 4:
            m_layout->removeWidget(m_editor);
            delete m_editor;
            m_editor = new DashboardEditor(m_dataFrame, this);
            m_layout->insertWidget(0, m_editor, 1);
            break;
        case 5:
            m_layout->removeWidget(m_editor);
            delete m_editor;
            m_editor = new GlobeEditor(m_dataFrame, this);
            m_layout->insertWidget(0, m_editor, 1);
            break;
        case 6:
            m_layout->removeWidget(m_editor);
            delete m_editor;
            m_editor = new MapEditor(m_dataFrame, this);
            m_layout->insertWidget(0, m_editor, 1);
            break;
        case 7:
            m_layout->removeWidget(m_editor);
            delete m_editor;
            m_editor = new AHEditor(m_dataFrame, this);
            m_layout->insertWidget(0, m_editor, 1);
            break;
    }
}

// makes the module float
void Module::makeFloating() {
    setFloating(true);
}

// splits the module into two modules, these are horizontally arranged
Module* Module::splitModuleHorizontally() {
    // let the parent workspace split the Modules
    if (m_workspace != nullptr){
        Module* newModule = new Module(m_dataFrame, m_workspace);
        m_workspace->addModule(newModule);
        m_workspace->splitDockWidget(this, newModule, Qt::Horizontal);

        return newModule;
    }
    return nullptr;
}

// splits the module into two modules, these are vertically arranged
Module* Module::splitModuleVertically() {
    // let the parent workspace split the Modules
    if (m_workspace != nullptr){
        Module* newModule = new Module(m_dataFrame, m_workspace);
        m_workspace->addModule(newModule);
        m_workspace->splitDockWidget(this, newModule, Qt::Vertical);

        return newModule;
    }
    return nullptr;
}

// change colors of statusbar
void Module::setStatusOK(QString message) {
    m_statusbar->setStyleSheet(QString("background-color: hsv(279, 94%, 89%);"));

    m_statusbar->setIcon(QIcon(":/icons/dark_OK.svg").pixmap(QSize(20, 20)));
    m_statusbar->setMessage(message);
}

void Module::setStatusSuccess(QString message) {
    m_statusbar->setStyleSheet(QString("background-color: hsv(117, 76%, 73%)"));

    m_statusbar->setIcon(QIcon(":/icons/dark_success.svg").pixmap(QSize(20, 20)));
    m_statusbar->setMessage(message);

    // the success status will only be shown shortly
    QTimer::singleShot(7000, [this](){setStatusOK(QString(""));});
}

void Module::setStatusWorking(QString message) {
    m_statusbar->setStyleSheet(QString("background-color: hsv(209, 84%, 97%);"));

    m_statusbar->setIcon(QIcon(":/icons/dark_working.svg").pixmap(QSize(20, 20)));
    m_statusbar->setMessage(message);
}

void Module::setStatusFail(QString message) {
    m_statusbar->setStyleSheet(QString("background-color: hsv(0, 87%, 79%);"));

    m_statusbar->setIcon(QIcon(":/icons/dark_error.svg").pixmap(QSize(20, 20)));
    m_statusbar->setMessage(message);
}


Editor* Module::getEditor(){
    return m_editor;
}
