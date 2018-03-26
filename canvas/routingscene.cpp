#include "routingscene.h"
#include "routewindow.h"

RoutingScene *routingScene = 0;

RoutingScene::RoutingScene():QGraphicsScene(0,0,STATSSCENE_WIDTH_DEFAULT, STATSSCENE_HEIGHT_DEFAULT)
{
    m_lastX = 0;
    m_lastY = 0;
//    m_lastTime = -1;
}

RoutingScene *
RoutingScene::getInstance()
{
    if(!routingScene)
    {
        routingScene = new RoutingScene;
    }
    return routingScene;
}

void
RoutingScene::addProxyWidgets(NodeNameMap nodeNameMap)
{
    clear();
    if(nodeNameMap.size() != 0)
    {
        qDebug()<<nodeNameMap.size();
      for(uint32_t i=0 ; i < nodeNameMap.size() ;i++)
        {
         QString content = nodeNameMap[i];
         QLabel * tb = new QLabel();
         tb->setText(parseXML(content));
         QFont f (tb->font ());
         f.setPointSizeF (12);
         tb->setFont (f);

         QGraphicsProxyWidget * pw = addWidget (tb);
         QFontMetrics fm (f);
         pw->setMaximumHeight (fm.height () * tb->text ().count ("\n"));
         pw->adjustSize ();

         qreal newX = m_lastX + pw->widget ()->width ();
          if (newX >= sceneRect ().right ())
             {
             m_lastX = 0;
             m_lastY += pw->widget ()->height () + INTERSTATS_SPACE;
             }

         pw->setPos (m_lastX, m_lastY);
          m_lastX = pw->pos ().x () + pw->widget ()->width () + INTERSTATS_SPACE;
          m_lastY = pw->pos ().y ();
          m_bottomY = m_lastY + pw->widget ()->height ();
          //qDebug (QString ("Last X" + QString::number (m_lastX) + " w:" + QString::number (pw->widget ()->width ())));
          adjustRect ();
         }
      m_lastX = 0;
      m_lastY = 0;
      m_bottomY = 0;
    }
    else if(nodeNameMap.size() == 0)
    {
        QLabel * tb = new QLabel();
        tb->setText("No node!!! \n");
        QFont f (tb->font ());
        f.setPointSizeF (32);
        tb->setFont (f);

        QGraphicsProxyWidget * pw = addWidget (tb);
        QFontMetrics fm (f);
        pw->setMaximumHeight (fm.height () * tb->text ().count ("\n"));
        pw->adjustSize ();
        pw->setPos(500,500);
    }
}


void
RoutingScene::adjustRect ()
{
  QRectF currentRect = sceneRect ();
  QRectF newRect = QRectF (currentRect.topLeft (), QPointF (currentRect.bottomRight ().x (), m_bottomY));
  setSceneRect (newRect);
}



QString
RoutingScene::parseXML(QString nodeName)
{
    //读取log文件
    QString fileName = "/home/zhang/Script/route/" + nodeName + "_route.log";
//    QString fileName = "/home/zhang/Script/route/node1_route.log";
    QString strAll = "node:  " + nodeName + "\n";
    QFile file(fileName);
    if(file.open((QIODevice::ReadOnly|QIODevice::Text)))
    {
        QTextStream stream(&file);
        strAll = strAll + stream.readAll();
    }
    strAll = strAll + "\n";
    return strAll;
}
