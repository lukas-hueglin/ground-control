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
#include "qcustomplot.h"


class GraphEditor : public Editor
{
    Q_OBJECT

public:
    GraphEditor(DataFrame *p_dataFrame, QWidget *parent = nullptr);

    void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
    void keyReleaseEvent(QKeyEvent *event) Q_DECL_OVERRIDE;

private:
    void setupDrawer();
    void setupGraph();

    void setGraphColor(QColor color);
    void setGraphColor(int index, QColor color);

    void onZoomed(QWheelEvent *event);

private:
    QGridLayout *layout;

    QCustomPlot *m_plot;

    QMap<QString, QCheckBox*> *checkBoxes;
    QMap<QString, QPushButton*> *colorButtons;

    QMap<QString, DataSeries*>* series;

    QDateTimeAxis *m_axisT;
    QValueAxis *m_axisY;

    QLabel *failLabel;

    bool properlySetup;
};

#endif // GRAPHEDITOR_H
