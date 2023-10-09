#ifndef ARTIFICIALHORIZON_H
#define ARTIFICIALHORIZON_H

#include <QWidget>

#include "dataframe.h"


class ArtificialHorizon : public QWidget
{
    Q_OBJECT
public:
    explicit ArtificialHorizon(DataFrame *p_dataFrame, QWidget *parent = nullptr);

public slots:
    void update(unsigned int p_time);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    DataFrame *m_dataFrame;

    QBitmap m_mainMask;
    QBitmap m_shieldMask;

    QPixmap m_overlay;

    QImage m_mainAHImg;
    QImage m_clearAHImg;
    QImage m_arrowImg;
    QImage m_compassImg;

    QImage m_scale01Img;
    QImage m_scale02Img;

    float m_pitch;
    float m_roll;
    float m_heading;
    float m_vel;
    float m_alt;

};

#endif // ARTIFICIALHORIZON_H
