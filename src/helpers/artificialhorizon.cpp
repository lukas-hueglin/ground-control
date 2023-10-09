#include "helpers/artificialhorizon.h"

#include <QPainter>
#include <QPaintEvent>
#include <QRegion>
#include <QDebug>

ArtificialHorizon::ArtificialHorizon(DataFrame *p_dataFrame, QWidget *parent)
    : QWidget{parent}, m_pitch{0}, m_roll{0}, m_heading{0}, m_vel{0}, m_alt{0}
{
    m_dataFrame = p_dataFrame;

    m_mainMask = QBitmap::fromImage(QImage(":/ah/artificialhorizon_main_mask.png").createAlphaMask());
    m_shieldMask = QBitmap::fromImage(QImage(":/ah/artificialhorizon_shield_mask.png").createAlphaMask());

    m_overlay = QPixmap::fromImage(QImage(":/ah/artificialhorizon_overlay.png"));

    m_mainAHImg = QImage(":/ah/artificialhorizon_main.png");
    m_clearAHImg = QImage(":/ah/artificialhorizon_clear.png");
    m_arrowImg = QImage(":/ah/artificialhorizon_arrow.png");
    m_compassImg = QImage(":/ah/artificialhorizon_compass.png");

    m_scale01Img = QImage(":/ah/artificialhorizon_scale01.png");
    m_scale02Img = QImage(":/ah/artificialhorizon_scale02.png");
}

void ArtificialHorizon::update(unsigned int p_time) {
    m_pitch = qRadiansToDegrees(m_dataFrame->getValue("pitch"));
    m_roll = qRadiansToDegrees(m_dataFrame->getValue("roll"));
    m_heading = qRadiansToDegrees(m_dataFrame->getValue("yaw"));
    m_vel = m_dataFrame->getValue("ground speed");
    m_alt = m_dataFrame->getValue("height");
}

void ArtificialHorizon::paintEvent(QPaintEvent *paintEvent){
    // create QPainter
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::SmoothPixmapTransform,true);
    painter.setRenderHint(QPainter::LosslessImageRendering,true);

    // get canvas size
    QRect rect = paintEvent->rect();
    QPoint center = rect.center();
    int side = qMin(rect.width(), rect.height())/1.6; //should be 2

    QRect canvas = QRect(center-QPoint(side, side), center+QPoint(side, side));

    // create QPixmap of main and clear artificial horizon
    QPixmap mainAH = QPixmap::fromImage(m_mainAHImg);
    QPixmap clearAH = QPixmap::fromImage(m_clearAHImg);

    // transform main artificial horizon
    QRect imageRect = mainAH.rect();
    mainAH = mainAH.transformed(QTransform().rotate(m_roll));
    clearAH = clearAH.transformed(QTransform().rotate(m_roll));

    // crop and apply mask
    int d = imageRect.height()*m_pitch/220; // 10 Deg distance in px
    QPoint imageCenter = mainAH.rect().center() + QPoint(qCos(qDegreesToRadians(90-m_roll))*d, -qSin(qDegreesToRadians(90-m_roll))*d);

    mainAH = mainAH.copy(QRect(imageCenter-QPoint(1024, 1024), imageCenter+QPoint(1023, 1023)));
    mainAH.setMask(m_mainMask);

    clearAH = clearAH.copy(QRect(imageCenter-QPoint(1024, 1024), imageCenter+QPoint(1023, 1023)));
    clearAH.setMask(m_shieldMask);

    // create Pixmap of arrow
    QPixmap arrow = QPixmap::fromImage(m_arrowImg);

    // rotate arrow
    arrow = arrow.transformed(QTransform().rotate(m_roll));
    arrow = arrow.copy(QRect(arrow.rect().center()-QPoint(1024, 1024), arrow.rect().center()+QPoint(1023, 1023)));

    // create Pixmap for compass
    QPixmap compass = QPixmap::fromImage(m_compassImg);
    compass = compass.transformed(QTransform().rotate(-m_heading));
    compass = compass.copy(QRect(compass.rect().center()-QPoint(1024, 2299), compass.rect().center()+QPoint(1023, -252)));

    // create Pixmap of bars
    QPixmap velInd = QPixmap::fromImage(m_scale01Img);
    QPixmap altInd = QPixmap::fromImage(m_scale01Img);

    // resize
    velInd = velInd.scaledToWidth(side/4);
    altInd = altInd.scaledToWidth(side/4);

    // velocity
    QRect velRect = velInd.rect();
    QPoint velCenter = QPoint(velRect.width()/2, velRect.height()*(1 - (m_vel/500*0.9738311 + 0.0186448)));

    QRect velSourceRect = QRect(velCenter-QPoint(velRect.width()/2, 512), velCenter+QPoint(velRect.width()/2, 512));
    QRect velTargetRect = velSourceRect;

    velTargetRect.moveCenter(center-QPoint(side*0.59, 0));

    // altitude
    QRect altRect = altInd.rect();
    QPoint altCenter = QPoint(altRect.width()/2, altRect.height()*(1 - (m_alt/50000*0.9738311 + 0.0186448)));

    QRect altSourceRect = QRect(altCenter-QPoint(altRect.width()/2, 512), altCenter+QPoint(altRect.width()/2, 512));
    QRect altTargetRect = altSourceRect;

    altTargetRect.moveCenter(center+QPoint(side*0.59, 0));

    // set clip rect
    painter.setClipRect(canvas);

    // paint
    painter.drawPixmap(canvas, mainAH);
    painter.drawPixmap(canvas, clearAH);
    painter.drawPixmap(canvas, arrow);
    painter.drawPixmap(canvas, compass);
    painter.drawPixmap(canvas, m_overlay);

    // set clip rect
    QRect clip = QRect(center-QPoint(side, side*0.5), center+QPoint(side, side*0.48));
    painter.setClipRect(clip);

    painter.drawPixmap(velTargetRect, velInd, velSourceRect);
    painter.drawPixmap(altTargetRect, altInd, altSourceRect);
}
