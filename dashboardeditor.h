#ifndef DASHBOARDEDITOR_H
#define DASHBOARDEDITOR_H

#include "editor.h"

#include <QCheckBox>
#include <QLabel>
#include <QGridLayout>
#include <QPushButton>
#include <QDockWidget>


struct DashboardLabel : public QDockWidget
{
    Q_OBJECT
public:
    DashboardLabel(DataFrame *p_dataFrame, QString strLabel, QString strValue, QWidget *parent = nullptr);

    DataFrame *m_dataFrame;

    QLabel* label;
    QLabel* value;
};


class DashboardEditor : public Editor
{
    Q_OBJECT
public:
    DashboardEditor(DataFrame *p_dataFrame, QWidget *parent = nullptr);

private:
    void setupDrawer();
    void setupDashboard();

    void toggleLabel(QString key, bool checked);

private:
    QGridLayout *layout;

    QMap<QString, QCheckBox*> *checkBoxes;
    QMap<QString, QPushButton*> *colorButtons;

    QMap<QString, DashboardLabel*>* labels;

    QLabel *failLabel;

    bool properlySetup;

};

#endif // DASHBOARDEDITOR_H
