#include "chartview.h"

#include <QDebug>

ChartView::ChartView(QChart *p_chart, QWidget *parent)
    : QChartView{p_chart, parent}
{
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

void ChartView::requestCallout(QPointF p_pos, QString p_text, bool state) {
    if (state && m_allowCallout) {
        m_callout->setAnchor(p_pos);
        m_callout->setText(p_text);
        m_callout->updateGeometry();
        m_callout->show();
    } else {
        m_callout->hide();
    }
}
