#include "chartview.h"
#include "qdatetime.h"

#include <QDebug>

ChartView::ChartView(DataFrame *p_dataFrame, QChart *p_chart, QWidget *parent)
    : QChartView{p_chart, parent}
{
    // set data frame
    m_dataFrame = p_dataFrame;

    // create QChart
    m_chart = p_chart;
    setRenderHint(QPainter::Antialiasing);

    // create callout
    m_callout = new Callout(m_chart);
    m_callout->hide();

    // set floats
    m_time0 = m_dataFrame->getDateTime(0)->toMSecsSinceEpoch();

    // set bools
    m_alignXAxis = false;
    m_allowCallout = false;
}

void ChartView::wheelEvent(QWheelEvent *event) {
    QRectF plotArea = m_chart->plotArea();
    QRectF rect = QRectF(m_chart->mapToValue(plotArea.topLeft()), m_chart->mapToValue(plotArea.bottomRight()));

    bool ctrl = event->modifiers().testFlag(Qt::ControlModifier);
    qreal factor = event->angleDelta().y() > 0 ? 0.8 : 1.25;

    QPointF center = m_chart->mapToValue(m_chart->mapFromScene(mapToScene(m_lastMousePos.toPoint())));

    center.setY(m_alignXAxis ? rect.center().y() : factor*rect.center().y());

    rect.setWidth(ctrl ? rect.width() : factor*rect.width());
    rect.setHeight(factor*rect.height());
    rect.moveCenter(center);

    if(rect.left() < m_time0) {
        rect.moveLeft(m_time0);
    }

    m_chart->zoomIn(QRectF(m_chart->mapToPosition(rect.topLeft()), m_chart->mapToPosition(rect.bottomRight())));

    event->accept();
    QChartView::wheelEvent(event);
}

void ChartView::mousePressEvent(QMouseEvent *event) {
    if (event->buttons() & Qt::LeftButton) {
        m_lastMousePos = event->pos();
        event->accept();
    }
    QChartView::mousePressEvent(event);
}

void ChartView::mouseMoveEvent(QMouseEvent *event) {
    if (event->buttons() & Qt::LeftButton) {
        QPointF dPos = m_chart->mapFromScene(event->pos()) - m_chart->mapFromScene(m_lastMousePos);

        float left = m_chart->mapToValue(m_chart->plotArea().topLeft()).x();
        float right = m_chart->mapToValue(m_chart->plotArea().bottomRight()).x();
        float unit = m_chart->plotArea().width() / (right - left);

        if (unit * (left - m_time0) < dPos.x()){
             m_chart->scroll(-unit * (left - m_time0), 0);
        } else {
            m_chart->scroll(-dPos.x(), 0);
        }

        m_lastMousePos = event->pos();
        event->accept();
    } else if (event->button() == Qt::NoButton) {
        m_lastMousePos = event->pos();

        m_callout->setAnchor(m_chart->mapToValue(event->pos()));
        m_callout->updateGeometry();
        event->accept();
    }
    QChartView::mouseMoveEvent(event);
}

void ChartView::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Control) {
        m_allowCallout = true;
        event->accept();
    }
    QChartView::keyPressEvent(event);
}

void ChartView::keyReleaseEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Control) {
        m_allowCallout = false;
        m_callout->hide();
        event->accept();
    }
    QChartView::keyReleaseEvent(event);
}

void ChartView::setXAxisAlign(bool p_state) {
    m_alignXAxis = p_state;
}

void ChartView::requestCallout(QPointF p_pos, QString p_key, bool state) {
    if (state && m_allowCallout) {
        m_callout->setAnchor(p_pos);
        m_callout->setKey(p_key);
        m_callout->updateGeometry();
        m_callout->show();
    } else {
        m_callout->hide();
    }
}
