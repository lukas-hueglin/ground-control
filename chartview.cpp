#include "chartview.h"
#include "qdatetime.h"

#include <QDebug>

ChartView::ChartView(DataFrame *p_dataFrame, QChart *p_chart, QWidget *parent)
    : QChartView{p_chart, parent}
{
    // set data frame
    m_dataFrame = p_dataFrame;

    m_chart = p_chart;
    setRenderHint(QPainter::Antialiasing);

    m_callout = new Callout(m_chart);

    m_callout->hide();
    m_allowCallout = false;
}

void ChartView::wheelEvent(QWheelEvent *event) {
    QRectF rect = m_chart->plotArea();
    QPointF center = rect.center();

    qreal factor = event->angleDelta().y() > 0 ? 0.8 : 1.25;

    rect.setWidth(factor*rect.width());
    rect.setHeight(event->modifiers().testFlag(Qt::ControlModifier) ? rect.height() : factor*rect.height());
    rect.moveCenter(center);

    m_chart->zoomIn(rect);

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
        m_chart->scroll(-dPos.x(), dPos.y());

        m_lastMousePos = event->pos();
        event->accept();
    } else if (event->button() == Qt::NoButton) {
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
