#include "editor.h"

#include <QPropertyAnimation>
#include <QHBoxLayout>
#include <QComboBox>
#include <QPushButton>


Editor::Editor(QWidget *parent)
    : QDockWidget{parent}
{
    // store parent
    workspace = qobject_cast<QMainWindow*>(parent);

    // create the titlebar
    QWidget *titlebar = new QWidget(this);
    setTitleBarWidget(titlebar);

    // create a QHBoxLayout
    QHBoxLayout *titleLayout = new QHBoxLayout;
    titlebar->setLayout(titleLayout);

    // add a QComboBox
    QComboBox *comboBox = new QComboBox(titlebar);
    comboBox->addItem(QString("Editor 1"));
    comboBox->addItem(QString("Editor 2"));
    comboBox->addItem(QString("Editor 3"));

    titleLayout->addWidget(comboBox);

    // add a QPushButton for splitting the Editor horizontally
    QPushButton *splitButtonH = new QPushButton(QString("Split Editor H"), titlebar);
    titleLayout->addWidget(splitButtonH);
    connect(splitButtonH, &QPushButton::pressed, this, &Editor::splitEditorHorizontally);

    // add a QPushButton for splitting the Editor vertically
    QPushButton *splitButtonV = new QPushButton(QString("Split Editor V"), titlebar);
    titleLayout->addWidget(splitButtonV);
    connect(splitButtonV, &QPushButton::pressed, this, &Editor::splitEditorVertically);

    // create QMainWindow and set as widget
    mainWindow = new QMainWindow;
    setWidget(mainWindow);

    // create viewport and add to central widget
    viewport = new QWidget(mainWindow);
    mainWindow->setCentralWidget(viewport);

    // create QGroupBox
    drawer = new QGroupBox(QString("Settings"), viewport);

    // create QToolButton
    drawerButton = new QToolButton(viewport);
    drawerButton->setArrowType(Qt::ArrowType::RightArrow);
    drawerButton->setCheckable(true);
    drawerButton->setChecked(false);

    // create QPropertyAnimations for QGroupWidget and QToolButton
    QPropertyAnimation *groupAnim = new QPropertyAnimation(drawer, "pos", viewport);
    QPropertyAnimation *buttonAnim = new QPropertyAnimation(drawerButton, "pos", viewport);

    // set position and size of widget and drawerButton
    drawer->setGeometry(-200, 0, 200, 500);
    drawerButton->setGeometry(0, 0, 15, 20);

    // set settings for groupAnim
    groupAnim->setDuration(200);
    groupAnim->setStartValue(QPoint(-drawer->width(), 0));
    groupAnim->setEndValue(QPoint(0, 0));

    // set settings for buttonAnim
    buttonAnim->setDuration(200);
    buttonAnim->setStartValue(QPoint(0, 0));
    buttonAnim->setEndValue(QPoint(drawer->width(), 0));

    // create QParallelAnimationGroup
    drawerAnim = new QParallelAnimationGroup;
    drawerAnim->addAnimation(groupAnim);
    drawerAnim->addAnimation(buttonAnim);

    // connect drawerButton to animations
    connect(drawerButton, &QToolButton::clicked, [this](const bool checked) {
        drawerButton->setArrowType(checked ? Qt::ArrowType::LeftArrow : Qt::ArrowType::RightArrow);
        drawerAnim->setDirection(checked ? QAbstractAnimation::Forward : QAbstractAnimation::Backward);
        drawerAnim->start();
    });
}


// splits the editor into two editors, these are horizontally arranged
void Editor::splitEditorHorizontally() {
    Editor *newEditor = new Editor(workspace);

    // let the parent workspace split the editors
    if (workspace != nullptr){
        workspace->splitDockWidget(this, newEditor, Qt::Horizontal);
    } else {

    }
}

// splits the editor into two editors, these are vertically arranged
void Editor::splitEditorVertically() {
    Editor *newEditor = new Editor(workspace);

    // let the parent workspace split the editors
    if (workspace != nullptr){
        workspace->splitDockWidget(this, newEditor, Qt::Vertical);
    } else {

    }
}

