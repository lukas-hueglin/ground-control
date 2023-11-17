#ifndef DRAWER_H
#define DRAWER_H

#include <QWidget>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QToolButton>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>

class Drawer : public QWidget
{
    Q_OBJECT

public:
    explicit Drawer(QWidget *parent = nullptr);

public:
    QGroupBox* getSettings();
    void setupContent(QLayout* p_layout);

private:
    QHBoxLayout* m_layout;

    QGroupBox* m_settings;
    QToolButton* m_button;

    QPropertyAnimation *m_propertyAnim;
    QParallelAnimationGroup* m_drawerAnim;
};

#endif // DRAWER_H
