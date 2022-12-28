#ifndef DASHBOARDEDITOR_H
#define DASHBOARDEDITOR_H

#include "editor.h"
#include "mainwindow.h"

#include <QCheckBox>
#include <QLabel>
#include <QGridLayout>
#include <QPushButton>


struct DashboardLabel : public QDockWidget
{
    Q_OBJECT
public:
    explicit DashboardLabel(QString strLabel, QString strValue, QWidget *parent = nullptr);

    QLabel* label;
    QLabel* value;
};


class DashboardEditor : public Editor
{
    Q_OBJECT
public:
    explicit DashboardEditor(Module *parent = nullptr, MainWindow *mainWindow = nullptr);

public:
    void updateTime(int t) override;

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
