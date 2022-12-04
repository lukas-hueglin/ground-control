#include "editor.h"

#include <QPropertyAnimation>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>

#include<QDebug>



Editor::Editor(QWidget *parent)
    : QWidget{parent}
{
    // create QLabel as viewport
    QLabel *label = new QLabel(QString("Empty"), parent);
    label->setAlignment(Qt::AlignCenter);

    // set viewport
    viewport = label;

    // st drawer up
    setupDrawer();
}

QWidget* Editor::getViewport() {
    return viewport;
}

void Editor::setupDrawer() {
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

