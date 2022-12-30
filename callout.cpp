#include "callout.h"

#include <QPainter>
#include <QDebug>

Callout::Callout(QChart *parent)
    : QGraphicsItem{parent}
{
    m_chart = parent;
    setAcceptHoverEvents(true);
}

QRectF Callout::boundingRect() const
{
    QPointF center = mapFromParent(m_chart->mapToPosition(m_anchor));
    QRectF rect = mapFromParent(m_chart->plotArea()).boundingRect();
    rect.setLeft(center.x()-9);
    rect.setRight(center.x()+9);

    return rect;
}

void Callout::setAnchor(QPointF p_anchor) {
    m_anchor = p_anchor;
}

void Callout::setText(QString p_text) {
    m_text = p_text;
}

void Callout::updateGeometry(){
    prepareGeometryChange();
    setPos(m_chart->mapToPosition(m_anchor));
}

void Callout::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
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
    center.setY(center.y()+1);

    elipRect.moveCenter(center.toPoint());
    painter->drawEllipse(elipRect);

    // draw vertical lines
    QPointF top = mapFromParent(QPointF(0, m_chart->plotArea().top()));
    QPointF bottom = mapFromParent(QPointF(0, m_chart->plotArea().bottom()));
    top.setX(center.x());
    bottom.setX(center.x());

    painter->drawLine(center-QPointF(0, 4.5), top);
    painter->drawLine(center+QPointF(0, 4.5), bottom);
}
