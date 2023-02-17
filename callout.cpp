#include "callout.h"

#include <QPainter>
#include <QDateTime>
#include <QStaticText>
#include <QDebug>

Callout::Callout(QChart *parent)
    : QGraphicsItem{parent}
{
    m_chart = parent;
    setAcceptHoverEvents(true);
    m_rect = QRect();

    setZValue(100);
}

QRectF Callout::boundingRect() const
{
    QPointF center = mapFromParent(m_chart->mapToPosition(m_anchor));
    QRectF rect = mapFromParent(m_chart->plotArea()).boundingRect();
    rect.setLeft(center.x()-9);
    rect.setRight(m_rect.right());

    return rect;
}

void Callout::setAnchor(QPointF p_anchor) {
    m_anchor = p_anchor;
}

void Callout::setKey(QString p_key) {
    m_key = p_key;
}

void Callout::updateGeometry(){
    prepareGeometryChange();
    setPos(m_chart->mapToPosition(m_anchor));
}

void Callout::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->setClipping(false);

    // create black pen
    QPen pen = QPen(QColor("black"));
    pen.setWidth(1);

    // create transparent Brush
    QBrush brush = QBrush(QColor(0, 0, 0, 0));

    // set pen and brush
    painter->setBrush(brush);
    painter->setPen(pen);

    // draw circle
    QRect elipRect(0, 0, 9, 9);
    QPointF center = mapFromParent(m_chart->mapToPosition(m_anchor));

    elipRect.moveCenter(center.toPoint());
    painter->drawEllipse(elipRect);

    // draw vertical lines
    QPointF top = mapFromParent(QPointF(0, m_chart->plotArea().top()));
    QPointF bottom = mapFromParent(QPointF(0, m_chart->plotArea().bottom()));
    top.setX(center.x());
    bottom.setX(center.x());

    painter->drawLine(center-QPointF(0, 4.5), top);
    painter->drawLine(center+QPointF(0, 4.5), bottom);

    // draw Box
    m_rect.setWidth(100);
    m_rect.setHeight(60);

    m_rect.moveCenter(center.toPoint());

    int sign = center.y() < mapFromParent(m_chart->plotArea().center()).y() ? 1 : -1;
    m_rect.translate(QPointF(m_rect.width()/2+10, sign*(m_rect.height()/2-10)).toPoint());

    brush.setColor(QColor(220, 220, 220, 220));
    painter->setBrush(brush);

    painter->drawRoundedRect(m_rect, 5, 5);

    // draw text
    QPoint keyPos = m_rect.topLeft() + QPoint(5, 15);
    QFont font = QFont("Sans Serif");

    font.setBold(true);
    painter->setFont(font);
    painter->drawText(keyPos, m_key);

    QRect textRect = QRect(keyPos+QPoint(0, 7), m_rect.bottomRight()-QPoint(5, -5));
    unsigned int time = (unsigned int) m_anchor.x()-1;
    float yValue = m_anchor.y();
    QDateTime *dateTime = new QDateTime();
    dateTime->setMSecsSinceEpoch(time);

    QString text = dateTime->toString("hh:mm:ss.zzz") + "\n" + QString::number(yValue);

    font.setBold(false);
    painter->setFont(font);
    painter->drawText(textRect, text);

}
