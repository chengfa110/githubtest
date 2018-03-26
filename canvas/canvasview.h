#ifndef RADARVIEW_H
#define RADARVIEW_H
#include <piperecevice.h>
#include <QGraphicsView>
#include <canvasnode.h>
#include <QReadWriteLock>
#include <canvasscene.h>

class canvasview : public QGraphicsView
{
    Q_OBJECT
public:
    explicit canvasview(QGraphicsScene * scene, QWidget * parent = 0);
    static canvasview * getInstance();
    int readPoint = 0;
    QReadWriteLock pipeLock;
    void nodeRePaint(nodePosition position1);
signals:

public slots:
    void timerEvent(QTimerEvent *event);
    void updateTransform ();
};

#endif // RADARVIEW_H
