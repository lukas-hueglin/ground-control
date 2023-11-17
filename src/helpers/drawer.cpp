#include "helpers/drawer.h"

Drawer::Drawer(QWidget* parent)
    : QWidget{parent}
{
    // create settings QGroupBox
    m_settings = new QGroupBox(QString("Settings"), this);
    m_settings->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));

    // create button QToolButton
    m_button = new QToolButton(this);
    m_button->setArrowType(Qt::ArrowType::RightArrow);
    m_button->setCheckable(true);
    m_button->setChecked(false);

    // create layout QHBoxLayout
    m_layout = new QHBoxLayout(this);
    m_layout->setContentsMargins(2, 0, 0, 0);
    m_layout->setSpacing(1);
    setLayout(m_layout);

    // add widgets to layout
    m_layout->addWidget(m_settings, 1, Qt::AlignLeft);
    m_layout->addWidget(m_button, 0, Qt::AlignTop);

    // create QPropertyAnimations for settings QGroupWidget
    m_propertyAnim = new QPropertyAnimation(m_settings, "minimumWidth", this);

    // set position and size of widget and m_button
    m_settings->setMaximumWidth(0);
    m_button->setFixedSize(15, 20);

    // set settings for propertyAnim
    m_propertyAnim->setDuration(200);
    m_propertyAnim->setStartValue(0);
    m_propertyAnim->setEndValue(150);

    // create QParallelAnimationGroup
    m_drawerAnim = new QParallelAnimationGroup(this);
    m_drawerAnim->addAnimation(m_propertyAnim);

    // connect m_button to animations
    connect(m_button, &QToolButton::clicked, [this](const bool checked) {
        m_button->setArrowType(checked ? Qt::ArrowType::LeftArrow : Qt::ArrowType::RightArrow);
        m_drawerAnim->setDirection(checked ? QAbstractAnimation::Forward : QAbstractAnimation::Backward);
        m_drawerAnim->start();
    });
}

/**
 * @brief getSettings
 * Returns the main widget inside the drawer class, the settings QGroupBox.
 * @return m_settings
 */
QGroupBox* Drawer::getSettings() {
    return m_settings;
}


/**
 * @brief setupContent
 * This method can be used to connect the already created widgets to the drawer.
 * The widgets should be assigned to a layout and should be parented by the drawer.
 * @param p_layout
 * The layout every widget is assigned to.
 */
void Drawer::setupContent(QLayout* p_layout) {
    m_settings->setLayout(p_layout);
}
