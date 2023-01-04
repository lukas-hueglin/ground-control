#ifndef CHARTVIEW_H
#define CHARTVIEW_H

#include <QChartView>
#include <QChart>

#include "callout.h"
#include "dataframe.h"

class ChartView : public QChartView
{
    Q_OBJECT

public:
    ChartView(DataFrame *p_dataFrame, QChart *p_chart, QWidget *parent = nullptr);

public:
    void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

    void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
    void keyReleaseEvent(QKeyEvent *event) Q_DECL_OVERRIDE;

    void setXAxisAlign(bool p_state);

public slots:
    void requestCallout(QPointF p_pos, QString p_key, bool state);

private:
    DataFrame *m_dataFrame;

    QChart *m_chart;
    Callout *m_callout;

    QPointF m_lastMousePos;

    float m_time0;

    bool m_alignXAxis;
    bool m_allowCallout;
};
#endif // CHARTVIEW_H
