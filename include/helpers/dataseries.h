#ifndef DATASERIES_H
#define DATASERIES_H

#include <QObject>
#include <QLineSeries>
#include <QAreaSeries>
#include <QValueAxis>
#include <QDateTimeAxis>
#include <QChart>

#include "dataframe.h"


struct DataSeries : public QObject
{
    Q_OBJECT

public:
    DataSeries(DataFrame *p_dataFrame, QString p_key, QDateTimeAxis *p_axisT, QValueAxis *p_axisY, QChart *parent = nullptr);

public slots:
    void setVisibility(bool p_show);
    void setColor(QColor p_color);

public:
    DataFrame *m_dataFrame;
    QString m_key;

    QLineSeries *m_series;
    QAreaSeries *m_area;
};

#endif // DATASERIES_H
