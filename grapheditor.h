#ifndef GRAPHEDITOR_H
#define GRAPHEDITOR_H

#include "editor.h"

#include <QCheckBox>
#include <QLabel>
#include <QGridLayout>
#include <QPushButton>
#include <QChart>
#include <QLineSeries>
#include <QAreaSeries>
#include <QValueAxis>
#include <QDateTimeAxis>
#include <QObject>

#include "dataseries.h"


class GraphEditor : public Editor
{
    Q_OBJECT

public:
    GraphEditor(DataFrame *p_dataFrame, QWidget *parent = nullptr);

private:
    void setupDrawer();
    void setupGraph();

private:
    QGridLayout *layout;

    QChart *m_chart;

    QMap<QString, QCheckBox*> *checkBoxes;
    QMap<QString, QPushButton*> *colorButtons;

    QMap<QString, DataSeries*>* series;

    QDateTimeAxis *m_axisT;
    QValueAxis *m_axisY;

    QLabel *failLabel;

    bool properlySetup;
};

#endif // GRAPHEDITOR_H
