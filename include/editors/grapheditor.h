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

#include "helpers/dataseries.h"
#include "helpers/qcustomplot.h"


class GraphEditor : public Editor
{
    Q_OBJECT

public:
    GraphEditor(DataFrame *p_dataFrame, QWidget *parent = nullptr);

    void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
    void keyReleaseEvent(QKeyEvent *event) Q_DECL_OVERRIDE;

protected slots:
    void setupViewport() override;
    void setupDrawer() override;

private:
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
};

#endif // GRAPHEDITOR_H
