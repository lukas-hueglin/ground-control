#ifndef CHARTVIEW_H
#define CHARTVIEW_H

#include <QChartView>
#include <QChart>

#include "callout.h"

class ChartView : public QChartView
{
    Q_OBJECT

public:
    ChartView(QChart *p_chart, QWidget *parent = nullptr);

public:
    void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

    void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
    void keyReleaseEvent(QKeyEvent *event) Q_DECL_OVERRIDE;

public slots:
    void requestCallout(QPointF p_pos, QString p_text, bool state);

private:
    QChart *m_chart;
    Callout *m_callout;

    QPointF m_lastMousePos;

    bool m_allowCallout;
};
#endif // CHARTVIEW_H
