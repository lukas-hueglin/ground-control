#include "dataseries.h"

#include <QDateTime>
#include <QTimer>

DataSeries::DataSeries(DataFrame *p_dataFrame, QString p_key, QDateTimeAxis *p_axisT, QValueAxis *p_axisY, QChart *parent)
    : QObject{parent}
{
    // set dataFrame
    m_dataFrame = p_dataFrame;

    // set key
    m_key = p_key;

    // create line series
    m_series = new QLineSeries(parent);

    // create lower line series
    QLineSeries *lowerSeries = new QLineSeries;
    lowerSeries->append(QPointF(m_dataFrame->getDateTime(0)->toMSecsSinceEpoch(), 0));
    lowerSeries->append(QPointF(m_dataFrame->getDateTime(m_dataFrame->getSize()-1)->toMSecsSinceEpoch(), 0));

    // create area series
    m_area = new QAreaSeries(m_series, lowerSeries);

    // attach area series
    parent->addSeries(m_area);
    m_area->attachAxis(p_axisT);
    m_area->attachAxis(p_axisY);

    // wait for all area series to be attached and then attach series (hitbox)
    QTimer::singleShot(50, [this, parent, p_axisT, p_axisY](){
        parent->addSeries(m_series);
        m_series->attachAxis(p_axisT);
        m_series->attachAxis(p_axisY);
    });


    // set colors
    setColor(QColor("blue"));

    // add data
    if (p_dataFrame != nullptr) {
        for (unsigned int t=0; t < m_dataFrame->getSize(); t+=100) {
            m_series->append(m_dataFrame->getDateTime(t)->toMSecsSinceEpoch(), m_dataFrame->getValue(m_key, t));
        }
    }
}

void DataSeries::setVisibility(bool p_show) {
    if (p_show) {

        m_series->show();
        m_area->show();
    } else {
        m_series->hide();
        m_area->hide();
    }
}

void DataSeries::setColor(QColor p_color) {
    // set colors
    QColor prim_color = QColor();
    prim_color.setHsv(p_color.hue(), p_color.saturation()*0.8, p_color.value(), 100);
    QColor sec_color = QColor();
    sec_color.setHsv(p_color.hue(), p_color.saturation()*0.8, p_color.value(), 30);

    // draw area
    QPen pen = QPen(p_color);
    pen.setWidth(2);
    m_series->setPen(pen);

    QLinearGradient gradient(QPointF(0, 0), QPointF(0, 1));
    gradient.setColorAt(0.0, prim_color);
    gradient.setColorAt(1.0, sec_color);
    gradient.setCoordinateMode(QGradient::ObjectBoundingMode);
    m_area->setBrush(gradient);
}
