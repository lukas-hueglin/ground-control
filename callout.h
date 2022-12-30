#ifndef CALLOUT_H
#define CALLOUT_H

#include <QGraphicsItem>
#include <QChart>
#include <QGraphicsSceneMouseEvent>

class Callout : public QGraphicsItem
{
public:
    Callout(QChart *parent);

public:
    void setAnchor(QPointF p_anchor);
    void setText(QString p_text);

    QRectF boundingRect() const;
    void updateGeometry();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,QWidget *widget);

private:
    QChart *m_chart;

    QPointF m_anchor;
    QRect m_rect;

    QString m_text;
};

#endif // CALLOUT_H
