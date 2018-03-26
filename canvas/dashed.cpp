#include "dashed.h"

Dashed::Dashed()
{
    setAcceptHoverEvents (true);
}

Dashed::~Dashed()
{

}

QRectF
Dashed::boundingRect() const
{
    QPainterPath path;
    path.addEllipse (QPointF (0, 0), 20, 20);

    return path.boundingRect();
}

void
Dashed::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPen pen;  // creates a default pen

    pen.setStyle(Qt::DashLine);
    pen.setWidth(0.5);
    pen.setBrush(Qt::green);
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::RoundJoin);

    painter->setPen(pen);

    QPainterPath path;
    path.addEllipse(QPointF (0, 0), 10, 10);
    painter->setBrush(QColor(144,238,144,70));
    painter->drawPath(path);

    QPainterPath bigCircle;
    bigCircle.addEllipse(QPointF (0, 0), 15, 15);
    QPainterPath smallCircle;
    smallCircle.addEllipse(QPointF (0, 0), 10, 10);
    path=bigCircle - smallCircle;
    painter->setBrush(QColor(255,255,0,70));
    painter->drawPath(path);

    bigCircle.addEllipse(QPointF (0, 0), 20, 20);
    smallCircle.addEllipse(QPointF (0, 0), 15, 15);
    path=bigCircle - smallCircle;
    painter->setBrush(QColor(255,0,0,70));
    painter->drawPath(path);

}
