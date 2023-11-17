#ifndef DASHBOARDEDITOR_H
#define DASHBOARDEDITOR_H

#include "editor.h"

#include <QCheckBox>
#include <QLabel>
#include <QGridLayout>
#include <QPushButton>
#include <QDockWidget>


struct Card : public QDockWidget
{
    Q_OBJECT
public:
    Card(DataFrame *p_dataFrame, QString strLabel, QString strValue, QWidget *parent = nullptr);

public:
    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

public:
    DataFrame *m_dataFrame;

    QLabel* m_label;
    QLabel* m_value;
};


class DashboardEditor : public Editor
{
    Q_OBJECT
public:
    DashboardEditor(DataFrame *p_dataFrame, QWidget *parent = nullptr);

protected slots:
    void setupViewport() override;
    void setupDrawer() override;

private:
    void toggleCard(QString key, bool checked);

private:
    QGridLayout *m_vpLayout;

    QMap<QString, QCheckBox*> *m_checkBoxes;
    QMap<QString, QPushButton*> *m_colorButtons;

    QMap<QString, Card*>* m_cards;
};

#endif // DASHBOARDEDITOR_H
