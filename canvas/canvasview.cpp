#include "canvasview.h"
#include <QAction>
#include <QMenu>
#include <math.h>
#include <QContextMenuEvent>
#include "mainwindow.h"
#include <iostream>
#include <QDebug>
#include <piperecevice.h>

QReadWriteLock pipeLock;
canvasview * pcanvasview = 0 ;
//extern nodePosition position;
canvasview::canvasview(QGraphicsScene *scene, QWidget*parent) :
    QGraphicsView(scene,parent)
{
    startTimer(10);
}

void
canvasview::timerEvent(QTimerEvent *event)
{
        if (pipeRecevice::getInstance()->getflag() == 1)
        {
        nodePosition position1 = pipeRecevice::getInstance()->getPosition();
        int i = position1.nodeId;
        std::cout<<"t:"<<position1.time<<"  id:"<<position1.nodeId<<"    x:"<<position1.x<<"    y"<<position1.y<<std::endl;
        canvasNode *pItem = canvasNodeMgr::getInstance()->getNode(i);
        pItem -> setPos(position1.x,position1.y);
        pItem->getDescription ()->setPos (pItem->sceneBoundingRect ().bottomRight ());
        pItem->getDashed()->setPos(pItem->sceneBoundingRect().center());
        pipeRecevice::getInstance()->setFlagZero();

        LinkManager::getInstance()->repairLinks(i);

        qreal positionX = position1.x;
        qreal positionY = position1.y;
        canvasNodeMgr::getInstance()->addAPosition(uint32_t(position1.nodeId),qreal(position1.time),QPointF(positionX,positionY));
        canvasPropertyBroswer::getInstance()->showNodePositionTable(true);
        }
}

//根据节点最大的移动范围确定视图的缩放比例
void
canvasview::updateTransform()
{
    QTransform transform;
    QRectF sceneBoundaryRect = QRectF(0,0,50,50);

    qreal minDimension = qMin (sceneBoundaryRect.width (), sceneBoundaryRect.height ());

    qreal xScale = viewport ()->width ()/minDimension;
    qreal yScale = viewport ()->height ()/minDimension;
    //qDebug (width (), "Width");
    //qDebug (height (), "height");
    qreal minScale = qMin (xScale, yScale);
    transform.scale (minScale, minScale);
    transform.translate(1200,1200);
    setTransform(transform);
}

canvasview *
canvasview::getInstance()
{
    if (!pcanvasview)
      {
        pcanvasview = new canvasview (canvasscene::getInstance ());
      }
    return  pcanvasview;
}
