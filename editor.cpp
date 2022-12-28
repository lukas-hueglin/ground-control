#include "editor.h"

#include <QPropertyAnimation>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>

#include<QDebug>

#include "module.h"



Editor::Editor(Module *parent, MainWindow *mainWindow)
    : QWidget{parent}
{
    // set parent
    module = parent;

    // set mainWindow
    this->mainWindow = mainWindow;

    // create QHBoxLayout
    container = new QHBoxLayout;
    setLayout(container);

    // create QLabel as viewport
    //QLabel *label = new QLabel(QString("Empty"), module);
    //label->setAlignment(Qt::AlignCenter);

    // set viewport
    //viewport = label;

    // add viewport to layout
    //container->addWidget(viewport);

    // set time
    time = 0;

    //setupDrawer(); // Disable Drawer
}

QWidget* Editor::getViewport() {
    return viewport;
}

void Editor::setupDrawer() {
    // create QGroupBox
    drawer = new QGroupBox(QString("Settings"), this);

    // add drawer to layout
    container->insertWidget(0, drawer);

    // create QToolButton
    drawerButton = new QToolButton(this);
    drawerButton->setArrowType(Qt::ArrowType::RightArrow);
    drawerButton->setCheckable(true);
    drawerButton->setChecked(false);

    // add drawer to layout
    container->insertWidget(1, drawerButton, 0, Qt::AlignTop);

    // create QPropertyAnimations for QGroupWidget
    QPropertyAnimation *groupAnim = new QPropertyAnimation(drawer, "maximumWidth", viewport);

    // set position and size of widget and drawerButton
    drawer->setMaximumWidth(0);
    drawerButton->setFixedSize(15, 20);

    // set settings for groupAnim
    groupAnim->setDuration(200);
    groupAnim->setStartValue(0);
    groupAnim->setEndValue(400);

    // create QParallelAnimationGroup
    drawerAnim = new QParallelAnimationGroup;
    drawerAnim->addAnimation(groupAnim);


    // connect drawerButton to animations
    connect(drawerButton, &QToolButton::clicked, [this](const bool checked) {
        drawerButton->setArrowType(checked ? Qt::ArrowType::LeftArrow : Qt::ArrowType::RightArrow);
        drawerAnim->setDirection(checked ? QAbstractAnimation::Forward : QAbstractAnimation::Backward);
        drawerAnim->start();
    });
}


void Editor::updateTime(int t){}
