#ifndef DASHED_H
#define DASHED_H
#include "QtWidgets"

class Dashed : public QGraphicsItem
{
public:
    Dashed();
    ~Dashed();
    QRectF boundingRect () const;
    void paint (QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

};

#endif // DASHED_H
