#include "editor.h"

#include <QPropertyAnimation>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>

#include "module.h"


Editor::Editor(DataFrame *p_dataFrame, QWidget *parent)
    : QWidget{parent}
{
    // set dataFrame
    m_dataFrame = p_dataFrame;

    // create QHBoxLayout
    m_container = new QHBoxLayout;
    setLayout(m_container);

    // connect statusbar
    Module *module = qobject_cast<Module*>(parent);

    connect(this, &Editor::onStatusChangeSuccess, module, &Module::setStatusSuccess);
    connect(this, &Editor::onStatusChangeWorking, module, &Module::setStatusWorking);
    connect(this, &Editor::onStatusChangeOK, module, &Module::setStatusOK);
    connect(this, &Editor::onStatusChangeFail, module, &Module::setStatusFail);
}

QWidget* Editor::getViewport() {
    return m_viewport;
}

void Editor::setupDrawer() {
    // create QGroupBox
    m_drawer = new QGroupBox(QString("Settings"), this);

    // add drawer to layout
    m_container->insertWidget(0, m_drawer);

    // create QToolButton
    m_drawerButton = new QToolButton(this);
    m_drawerButton->setArrowType(Qt::ArrowType::RightArrow);
    m_drawerButton->setCheckable(true);
    m_drawerButton->setChecked(false);

    // add drawer to layout
    m_container->insertWidget(1, m_drawerButton, 0, Qt::AlignTop);

    // create QPropertyAnimations for QGroupWidget
    QPropertyAnimation *groupAnim = new QPropertyAnimation(m_drawer, "maximumWidth", m_viewport);

    // set position and size of widget and drawerButton
    m_drawer->setMaximumWidth(0);
    m_drawerButton->setFixedSize(15, 20);

    // set settings for groupAnim
    groupAnim->setDuration(200);
    groupAnim->setStartValue(0);
    groupAnim->setEndValue(400);

    // create QParallelAnimationGroup
    m_drawerAnim = new QParallelAnimationGroup;
    m_drawerAnim->addAnimation(groupAnim);


    // connect drawerButton to animations
    connect(m_drawerButton, &QToolButton::clicked, [this](const bool checked) {
        m_drawerButton->setArrowType(checked ? Qt::ArrowType::LeftArrow : Qt::ArrowType::RightArrow);
        m_drawerAnim->setDirection(checked ? QAbstractAnimation::Forward : QAbstractAnimation::Backward);
        m_drawerAnim->start();
    });
}

