#include "canvasscene.h"
#include <canvasconstants.h>
#include <iostream>
#include <QDebug>
#include <mainwindow.h>

canvasscene *pCanvasscene = 0;
QVector<canvasNode *> m_canvasNodes;
//QGraphicsScene
canvasscene::canvasscene():
    QGraphicsScene (0,0, ANIMATORSCENE_USERAREA_WIDTH, ANIMATORSCENE_USERAREA_WIDTH),
      m_enableMousePositionLabel (true)
{
    lineVisiable=true;
    m_nGridLines = GRID_LINES_DEFAULT;
    m_mousePositionLabel = new QLabel;
    m_mousePositionLabel->setSizePolicy (QSizePolicy::Minimum, QSizePolicy::Minimum);
    m_mousePositionProxyWidget = addWidget (m_mousePositionLabel, Qt::ToolTip);
    m_mousePositionProxyWidget->setFlag (QGraphicsItem::ItemIgnoresTransformations);
    readFile();
}

//画网格线
void
canvasscene::addGrid()
{
//    qreal maximum = qMax (m_maxPoint.x (), m_maxPoint.y ());
    QRectF gridRect (QPointF (0,0), QPointF (50, 50));
    qreal xStep = (gridRect.right ())/ (m_nGridLines-1);
    qreal yStep = (gridRect.bottom ())/ (m_nGridLines-1);
    //xStep = ceil (xStep);
    //yStep = ceil (yStep);
    QPen pen (QColor (0,0,0));
    pen.setWidth(2);
    pen.setCosmetic (true);

    // draw horizontal grid
    qreal y = 0;
    qreal x = 0;
    for (int c = 0; c < m_nGridLines; ++c, y += yStep)
      {
        m_gridLines.push_back (addLine (0, y, gridRect.right (), y, pen));
      }
    // now draw vertical grid
    for (int c = 0; c < m_nGridLines; ++c, x += xStep)
      {
        m_gridLines.push_back (addLine (x, 0, x,  gridRect.bottom (), pen));
      }

    initGridCoordinates ();
    markGridCoordinates ();

}

//初始化节点间连线的二位数组
void
canvasscene::InitializationLineVector(int count)
{
    for(int i=0;i<count;i++)
    {
        QVector <int> x;
        for(int j=0;j<count;j++)
        {
            x.push_back(1);
        }
        LineVector.push_back(x);
    }
//    qDebug()<<LineVector;
}

//修改节点间连线的二位数组
void
canvasscene::addLineVector(int x, int y)
{
    if(LineVector[x][y] == 0)
    {
        LineVector[x][y] = 1;
        LineVector[y][x] = 1;
        if(x < y)
            LinkManager::getInstance()->getLineitem(x,y)->setVisible(true);
        else if(y < x)
            LinkManager::getInstance()->getLineitem(y,x)->setVisible(true);
    }
}

void
canvasscene::deleteLineVector(int x, int y)
{
    if(LineVector[x][y] == 1)
    {
        LineVector[x][y] = 0;
        LineVector[y][x] = 0;
        if(x < y)
            LinkManager::getInstance()->getLineitem(x,y)->setVisible(false);
        else if(y < x)
            LinkManager::getInstance()->getLineitem(y,x)->setVisible(false);
    }
}
//画节点间连接线
void
canvasscene::drawLine(int count)
{
    for(int i=0;i<count;i++)
    {
        for(int j=0;j<count;j++)
        {
            if(i < j && LineVector[i][j]==1 )
            {
                lineitem * lineitem = 0;
                lineitem = LinkManager::getInstance ()->getLineitem (i,j);
                if (!lineitem)
                  {
                    lineitem = LinkManager::getInstance ()->addLink (i,j);
                    addItem(lineitem);
                  }
                 m_lineItems.push_back(lineitem);
            }
        }
    }
    deleteLine(count);
}

//隐藏连线
void
canvasscene::deleteLine(int count)
{
    lineVisiable=false;
    for(int i=0;i<count;i++)
    {
        for(int j=0;j<count;j++)
        {
            if(i < j && LineVector[i][j]==1)
            {
                LinkManager::getInstance()->getLineitem(i,j)->setVisible(lineVisiable);
            }
        }
    }
}

//显示直线
void
canvasscene::showLine(int count)
{
    lineVisiable=true;
    for(int i=0;i<count;i++)
    {
        for(int j=0;j<count;j++)
        {
            if(i < j && LineVector[i][j]==1)
            {
                LinkManager::getInstance()->getLineitem(i,j)->setVisible(lineVisiable);
            }
        }
    }
}

QGraphicsLineItem *
canvasscene::getLine(int id)
{
    return m_lineItems[id];
}

//网格线坐标点
void
canvasscene::initGridCoordinates ()
{
  for (int i = 0; i < m_gridCoordinates.size (); ++i)
    {
      QGraphicsSimpleTextItem * item = m_gridCoordinates[i];
      removeItem (item);
      delete item;
    }
  m_gridCoordinates.clear ();
  for (int i = 0; i < 9; i++) // only 9 coordinates will be marked
    {
      QGraphicsSimpleTextItem * item = new QGraphicsSimpleTextItem;
      item->setFlag (QGraphicsItem::ItemIgnoresTransformations);
      m_gridCoordinates.push_back (item);
      addItem (item);
    }
  markGridCoordinates ();

}

//在场景中显示网格坐标点
void
canvasscene::markGridCoordinates ()
{
//    QPointF m_minPoint;
//    QPointF m_maxPoint;
    QRectF simulationRect (0,0,50,50);
    if ((simulationRect.width () == 0) && (simulationRect.height () == 0))
      return;
    int i = 0;
    for (qreal x = 0; x <= simulationRect.right () ; x = x + (simulationRect.right ()/2))
    {
      for (qreal y = 0; y <= simulationRect.bottom () ; y = y + (simulationRect.bottom ()/2))
        {
          if (i == 9)
            return;
          QString text = QString::number (x, 'f', 1)
                         + ","
                         + QString::number (y, 'f', 1);
          m_gridCoordinates[i]->setText (text);
          m_gridCoordinates[i]->setPos (QPointF (x, y));
          m_gridCoordinates[i]->setVisible (true);
          i++;
        }
    }
}

//鼠标指针位置显示坐标
void canvasscene::setMousePositionLabel (QPointF pos)
{
    QString string = "    (" + QString::number ( (pos.x ())) + "," + QString::number ( (pos.y ())) + ")";

    m_mousePositionLabel->setText (string);
    m_mousePositionProxyWidget->setPos (pos.x (), pos.y ());
    m_mousePositionLabel->adjustSize ();
    m_mousePositionProxyWidget->setVisible (true);
}

//鼠标移动事件，显示坐标
void canvasscene::mouseMoveEvent (QGraphicsSceneMouseEvent *event)
{
    if (m_enableMousePositionLabel)
      {
        QPointF scenePos = event->scenePos ();
      //   QString s = "Mouse:" + QString::number (event->scenePos ().x ()) + "," + QString::number (event->scenePos ().y ());
      //   qDebug (s.toAscii ().data ());
        setMousePositionLabel (scenePos);
//        if ((scenePos.x () < 0) ||
//            (scenePos.y () < 0))
//          {
//            showMousePositionLabel (false);
//          }
//        else
//          {
            showMousePositionLabel (true);
//          }
      }
    return QGraphicsScene::mouseMoveEvent (event);
}

void
canvasscene::showMousePositionLabel (bool show)
{
  m_mousePositionProxyWidget->setVisible (show);
}

void
canvasscene::addNode (canvasNode *canvasNode)
{
    if(canvasNode->data(Qt::UserRole).toInt() == 0)
    {
        addItem(canvasNode->getDashed());
        canvasNode->getDashed()->setZValue(1);
        canvasNode->getDashed()->setPos(canvasNode->sceneBoundingRect().center());
    }
    addItem (canvasNode);
    canvasNode->setZValue(2);
    m_canvasNodes.push_back (canvasNode);

    if(canvasNode->data(Qt::UserRole).toInt() != 0)
        canvasNode->setSize(1,1);
    else
        canvasNode->setSize(3,3);
//  qDebug()<<m_canvasNodes[0]->type();
    canvasNode->setPos (canvasNode->getX (), canvasNode->getY ());
    addItem (canvasNode->getDescription ());
    canvasNode->getDescription ()->setZValue(2);
    canvasNode->getDescription ()->setPos (canvasNode->sceneBoundingRect ().bottomRight ());
//  qDebug()<<canvasNode->data(Qt::UserRole).toInt();
//  qDebug()<<canvasNode->getWidth();

}

void
canvasscene::readFile()
{
    QFile file("/home/zhang/Code/canvas/IP_and_MAC.txt");
    if(file.open((QIODevice::ReadOnly|QIODevice::Text)))
    {
        QTextStream stream(&file);
        QString line;
        do
        {
            line = stream.readLine();
            if (!line.isEmpty())
            {
                QStringList pieces = line.split(" ", QString
                       ::SkipEmptyParts);
                nodeInfo.push_back(pieces);
            }
        } while (!line.isEmpty());
            file.close();
         }
}

QVector<QStringList> canvasscene::getNodeInfo()
{
    return nodeInfo;
}

//节点初始位置
void canvasscene::initialPosition(QString count)
{
    int row=5;
    for(int i = 0 ; i < count.toInt() ; i++)
    {
        if(i*3+10<40)
        {
           QPointF point =QPointF(i*3+10,row);
           nodeinitialPos.push_back(point);
        }
        else
        {
            row = row + 5;
            QPointF point =QPointF(i*3+10,row);
            nodeinitialPos.push_back(point);
        }
    }
}

QPointF canvasscene::getInitialPosition(int i)
{
    return nodeinitialPos.at(i);
}

//修改初始位置数据
void canvasscene::InitialPositionChange(int row,int column, qreal data)
{
    if(column == 4)
    {
        QPoint point = QPoint(data,nodeinitialPos.at(row).y());
        setNodePos(canvasNodeMgr::getInstance()->getNode(row),point.x(),point.y());
        nodeinitialPos.replace(row,point);
    }
    else if(column == 5)
    {
        QPoint point = QPoint(nodeinitialPos.at(row).x(),data);
        setNodePos(canvasNodeMgr::getInstance()->getNode(row),point.x(),point.y());
        nodeinitialPos.replace(row,point);
    }
}

canvasscene*
canvasscene::getInstance()
{
    if (!pCanvasscene)
      {
        pCanvasscene = new canvasscene;
      }
    return pCanvasscene;
}

void
canvasscene::setNodePos (canvasNode *animNode, qreal x, qreal y)
{
  //NS_LOG_DEBUG ("SetNodePos:" << animNode->getNodeId ());
  animNode->setX (x);
  animNode->setY (y);
  animNode->setPos (x, y);
  animNode->getDescription ()->setPos (animNode->sceneBoundingRect ().bottomRight ());
  animNode->getDashed()->setPos(animNode->sceneBoundingRect().bottomRight ());
  LinkManager::getInstance ()->repairLinks (animNode->getNodeId ());
}

QVector<canvasNode *> canvasscene::getCanvasNodes()
{
    return m_canvasNodes;
}

void
canvasscene::removeNodes(int count)
{
    for(int i=0;i< count ;i++)
    {
        qDebug()<<m_canvasNodes.size();
        removeItem(m_canvasNodes.at(m_canvasNodes.size()-1)->getDescription());
        removeItem(m_canvasNodes.at(m_canvasNodes.size()-1));
        m_canvasNodes.pop_back();
        canvasNodeMgr::getInstance()->getm_nodes().erase(canvasNodeMgr::getInstance()->getm_nodes().end());
    }
}

void
canvasscene::setShowNodeTrajectory (canvasNode *animNode)
{
//  uint32_t nodeId = animNode->getNodeId ();
//  if (m_nodeTrajectory.find (nodeId) == m_nodeTrajectory.end ())
//    {
//      QPainterPath path;
//      canvasNodeMgr::TimePosVector_t positions =  canvasNodeMgr::getInstance ()->getPositions (animNode->getNodeId ());
//      for (AnimNodeMgr::TimePosVector_t::const_iterator i = positions.begin ();
//          i != positions.end ();
//          ++i)
//        {
//          TimePosition_t tp = *i;
//          QPointF pt = tp.p;
//          path.moveTo (pt);
//          break;
//        }


//      positions =  canvasNodeMgr::getInstance ()->getPositions (animNode->getNodeId ());
//      for (canvasNodeMgr::TimePosVector_t::const_iterator i = positions.begin ();
//          i != positions.end ();
//          ++i)
//        {
//          TimePosition_t tp = *i;
//          QPointF pt = tp.p;
//          path.lineTo (pt);
//        }
//      QGraphicsPathItem * pathItem = addPath (path);
//      QPen pen;
//      pen.setCosmetic (true);
//      pathItem->setPen (pen);
//      m_nodeTrajectory[nodeId] = pathItem;
//    }
//  m_nodeTrajectory[nodeId]->setVisible (animNode->getShowNodeTrajectory ());
}
