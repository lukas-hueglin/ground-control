#include "drawer.h"

Drawer::Drawer(QWidget *parent)
    : QWidget{parent}
{
    // create QGridLayout
    layout = new QGridLayout(this);
    setLayout(layout);

    // setup widget
    setGeometry(0, 0, 500, 500);


    // create and setup QToolButton
    toggleButton = new QToolButton;
    toggleButton->setArrowType(Qt::ArrowType::RightArrow);
    toggleButton->setCheckable(true);
    toggleButton->setChecked(false);

    // add QToolButton to QGridLayout
    layout->addWidget(toggleButton, 0, 0, Qt::AlignLeft);

    // create and setup animations
    toggleAnimation = new QPropertyAnimation;

    //QPropertyAnimation *minimumAnim = new QPropertyAnimation(this, "pos");
    //QPropertyAnimation *maximumAnim = new QPropertyAnimation(this, "maximumWidth");

    //minimumAnim->setDuration(1000);
    //minimumAnim->setStartValue(0);
    //minimumAnim->setEndValue(500);

    //maximumAnim->setDuration(1000);
    //maximumAnim->setStartValue(0);
    //maximumAnim->setEndValue(500);

    toggleAnimation->setDuration(1000);
    toggleAnimation->setStartValue(QPoint(0, 100));
    toggleAnimation->setEndValue(QPoint(100, 100));

    //toggleAnimation->addAnimation(minimumAnim);
    //toggleAnimation->addAnimation(maximumAnim);

    connect(toggleButton, &QToolButton::clicked, [this](const bool checked) {
        toggleButton->setArrowType(checked ? Qt::ArrowType::LeftArrow : Qt::ArrowType::RightArrow);
        toggleAnimation->setDirection(checked ? QAbstractAnimation::Backward : QAbstractAnimation::Forward);
        toggleAnimation->start();
    });

    setStyleSheet("background-color:red;");
}
