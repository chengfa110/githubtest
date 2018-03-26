#include <canvasnode.h>
#include <QColor>

canvasNodeMgr * pcanvasNodeMgr = 0;

canvasNode::canvasNode (uint32_t nodeId, uint32_t nodeSysId, qreal x, qreal y, QString nodeDescription):
  m_nodeDescription (0),
  m_nodeId (nodeId),
  m_nodeSysId (nodeSysId),
  m_x (x),
  m_y (y),
  m_showNodeId (true),
  m_showNodeSysId (false),
  m_resourceId (-1),
  m_showNodeTrajectory (false),
  m_showBatteryCapcity (false)
{
  //setVisible (false);
  setZValue (canvasNode_ZVALUE);
  m_r = 255;
  m_g = 0;
  m_b = 0;
  if (nodeDescription == "")
    {
      nodeDescription = QString::number (nodeId);
    }
  m_dashed = new Dashed;
  m_nodeDescription = new QGraphicsTextItem (nodeDescription);
  m_nodeDescription->setFlag (QGraphicsItem::ItemIgnoresTransformations);
  setFlag (QGraphicsItem::ItemIsSelectable);
  setFlags(ItemIsSelectable | ItemIsMovable);//设置鼠标移动，若需要使得范围圈圈一起动，需要（当鼠标点击QGraphicsItem后，移动鼠标，在移动鼠标的消息中计算移动信息，
//                                                然后实时设置QGraphicsItem移动的位置)
}

canvasNode::~canvasNode ()
{
  if (m_nodeDescription)
    {
      delete m_nodeDescription;
    }
}

void
canvasNode::showNodeId (bool show)
{
  m_showNodeId = show;
  m_nodeDescription->setVisible (m_showNodeId);
}

QColor
generateColor (size_t index, uint8_t alpha = 0)
{
  static const size_t colors[] =
    { Qt::blue, Qt::magenta, Qt::darkCyan, Qt::darkYellow, Qt::darkRed, Qt::darkMagenta, Qt::darkGreen, Qt::darkBlue,
    Qt::black, Qt::darkGray, Qt::lightGray };
  static const size_t COUNT = sizeof (colors) / sizeof (size_t);
  QColor result;

  if (index < COUNT)
    result = QColor (Qt::GlobalColor (colors[index]));
  else
    {
      result = QColor (Qt::GlobalColor (colors[index % COUNT]));
      const int step = 256 * 3 % COUNT;

      result.setRed ((result.red () + step * index) % 255);
      result.setGreen ((result.blue () + step * index) % 255);
      result.setBlue (abs ((int)result.green () - step * index) % 255);
    }
  if (alpha)
    result.setAlpha (alpha);

  return result;
}

void
canvasNode::showNodeSysId (bool show)
{
  if (show)
    {
      m_lastColor = this->getColor ();
      int r, g, b, a;
      m_lastColor.getRgb(&r, &g, &b, &a);
      const QColor &color = generateColor (m_nodeSysId, a);
      color.getRgb (&r, &g, &b, &a);
      setColor (static_cast<uint8_t> (r), static_cast<uint8_t> (g), static_cast<uint8_t> (b), static_cast<uint8_t> (a));
    }
  else
    {
      if(m_showNodeSysId)
    {
      int r, g, b, a;
      m_lastColor.getRgb (&r, &g, &b, &a);
      setColor (r, g, b, a);
    }
    }
  m_showNodeSysId = show;
  m_nodeDescription->setPlainText (QString::number (m_nodeId) + (m_showNodeSysId ? QString(" SysId:") +
  QString::number (m_nodeSysId): QString()));
}

bool
canvasNode::isVisibleNodeSysId() const
{
  return m_showNodeSysId;
}

void
canvasNode::updateNodeSysId (uint32_t sysId, bool show)
{
  m_nodeSysId = sysId;
//  m_nodeSysIdDescription->setPlainText ("sysId: " + QString::number (sysId));
  showNodeSysId (show);
}

//void
//canvasNode::updateBatteryCapacityImage (bool show)
//{

//  m_showBatteryCapcity = show;
//  QString batteryCapacityImagePath(":/resources/battery_icon_");
//  bool result = false;
//  CounterType_t counterType;
//  uint32_t counterId = canvasNodeMgr::getInstance ()->getCounterIdForName ("RemainingEnergy", result, counterType);
//  if (!result)
//    {
//      return;
//    }

//  result = false;
//  qreal capacity = getDoubleCounterValue (counterId, result);
//  if (!result)
//    {
//      return;
//    }

//  if (capacity > 0.75) batteryCapacityImagePath += "4";
//  else if (capacity > 0.5) batteryCapacityImagePath += "3";
//  else if (capacity > 0.25) batteryCapacityImagePath += "2";
//  else if (capacity >= 0) batteryCapacityImagePath += "1";
//  else batteryCapacityImagePath += "0";
// // NS_LOG_DEBUG ("Capacity:" << batteryCapacityImagePath.toAscii().data());

//  batteryCapacityImagePath += ".png";
//  if(show)
//    {
//      m_batteryPixmap = QPixmap (batteryCapacityImagePath);
//    }
//  if (!show)
//    {
//      m_batteryPixmap = QPixmap ();
//    }

//}



uint32_t
canvasNode::getUint32CounterValue (uint32_t counterId, bool & result)
{
  result = false;
  if (m_counterIdToValuesUint32.find (counterId) == m_counterIdToValuesUint32.end ())
    return -1;
  result = true;
  return m_counterIdToValuesUint32[counterId];
}


qreal
canvasNode::getDoubleCounterValue (uint32_t counterId, bool & result)
{
  result = false;
  if (m_counterIdToValuesDouble.find (counterId) == m_counterIdToValuesDouble.end ())
    return -1;
  result = true;
  return m_counterIdToValuesDouble[counterId];
}

void
canvasNode::updateCounter (uint32_t counterId, qreal counterValue, CounterType_t counterType)
{
  if (counterType == DOUBLE_COUNTER)
    {
      m_counterIdToValuesDouble[counterId] = counterValue;
    }

  if (counterType == UINT32_COUNTER)
    {
      m_counterIdToValuesUint32[counterId] = counterValue;
    }
}



int
canvasNode::getResourceId ()
{
  return m_resourceId;
}


//void
//canvasNode::setResource (int resourceId)
//{
//  m_resourceId = resourceId;
//  QString resourcePath = AnimResourceManager::getInstance ()->get (resourceId);
//  //NS_LOG_DEBUG ("Res:" << resourcePath.toAscii ().data ());
//  QPixmap pix;
//  if (resourcePath.endsWith (".png"))
//    pix = QPixmap (resourcePath, "png");
//  else if (resourcePath.endsWith (".svg"))
//    pix = QPixmap (resourcePath, "svg");
//  setPixmap (pix);
//  update ();
//}

void
canvasNode::setColor (uint8_t r, uint8_t g, uint8_t b, uint8_t alpha)
{
  m_r = r;
  m_g = g;
  m_b = b;
  m_alpha = alpha;
  ResizeableItem::setColor (r, g, b, alpha);
  update ();
}

QColor
canvasNode::getColor ()
{
  QColor c (m_r, m_g, m_b, 255);
  return c;
}

qreal
canvasNode::getWidth ()
{
  return m_width;
}



qreal
canvasNode::getX ()
{
  return m_x;
}

qreal
canvasNode::getY ()
{
  return m_y;
}


void
canvasNode::setPos (qreal x, qreal y)
{
  m_x = x;
  m_y = y;
  QGraphicsItem::setPos (x, y);
}

void
canvasNode::setX (qreal x)
{
  m_x = x;
}


bool
canvasNode::getShowNodeTrajectory ()
{
  return m_showNodeTrajectory;
}

void
canvasNode::setShowNodeTrajectory (bool showNodeTrajectory)
{
  m_showNodeTrajectory = showNodeTrajectory;
}


void
canvasNode::setY (qreal y)
{
  m_y = y;
}

uint32_t
canvasNode::getNodeId ()
{
  return m_nodeId;
}

uint32_t
canvasNode::getNodeSysId ()
{
  return m_nodeSysId;
}

QGraphicsTextItem *
canvasNode::getDescription ()
{
  return m_nodeDescription;
}

Dashed *
canvasNode::getDashed()
{
    return m_dashed;
}

QPointF canvasNode::getCenter ()
{
  //return sceneBoundingRect ().center ();
  return QPointF (m_x, m_y);
}
void canvasNode::setNodeDescription (QString description)
{
  m_nodeDescription->setPlainText (description);
}

void canvasNode::paint (QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if(data(Qt::UserRole).toInt() != 0)
    {
        ResizeableItem::paint (painter, option, widget);
    }
    else if(data(Qt::UserRole).toInt() == 0)
    {
        QPixmap wifi;
        wifi.load(":/new/canvas/canvasResource/wifi.png");
        setType(ResizeableItem::PIXMAP);
        setPixmap(wifi);
        ResizeableItem::paint (painter, option, widget);
    }
  if (!m_batteryPixmap.isNull ())
    {
//      updateBatteryCapacityImage (m_showBatteryCapcity);
      QPointF bottomLeft = sceneBoundingRect ().bottomLeft ();
      //NS_LOG_DEBUG ("Pix Width:" << m_batteryPixmap->width());
      bottomLeft = QPointF (-1, 1);
      painter->save ();
//      painter->(QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::TextAntialiasing | QPainter::HighQualityAntialiasing | QPainter::NonCosmeticDefaultPen, true);      painter->scale (0.5, 1);
      painter->drawPixmap (bottomLeft.x (), bottomLeft.y (), 1, 1, m_batteryPixmap);
      painter->restore ();
    }
}


void canvasNode::mouseMoveEvent (QGraphicsSceneMouseEvent *event)
{
  ResizeableItem::mouseMoveEvent (event);
  if (m_nodeDescription)
    {
//      qDebug()<<sceneBoundingRect ().bottomRight ();
      m_nodeDescription->setPos (sceneBoundingRect ().bottomRight ());
      update ();
    }
}

canvasNode::Ipv4Vector_t
canvasNode::getIpv4Addresses ()
{
  return m_ipv4Vector;
}

canvasNode::MacVector_t
canvasNode::getMacAddresses ()
{
  return m_macVector;
}

void
canvasNode::addIpv4Address (QString ip)
{
  m_ipv4Vector.push_back (ip);
}

void
canvasNode::addMacAddress (QString mac)
{
  m_macVector.push_back (mac);
}

bool
canvasNode::hasIpv4 (QString ip)
{
  bool result = false;
  QStringList quads = ip.split (".");
  if (quads.count () == 4)
    {
      if (quads.at (3) == "255")
        return true;
      for (Ipv4Vector_t::const_iterator i = m_ipv4Vector.begin ();
          i != m_ipv4Vector.end ();
          ++i)
        {
          if (*i == ip)
            {
              //QDEBUG (ip);
              return true;
            }
        }
    }

  return result;
}


bool
canvasNode::hasMac (QString mac)
{
  bool result = false;
  QStringList bytes = mac.split (":");
  if (bytes.count () == 6)
    {
      for (MacVector_t::const_iterator i = m_macVector.begin ();
          i != m_macVector.end ();
          ++i)
        {
          if (*i == mac)
            {
              return true;
            }
        }
    }

  return result;
}


canvasNodeMgr::canvasNodeMgr ():
  m_minX (0),
  m_minY (0),
  m_maxX (0),
  m_maxY (0)
{

}

canvasNodeMgr * canvasNodeMgr::getInstance ()
{
  if (!pcanvasNodeMgr)
    {
      pcanvasNodeMgr = new canvasNodeMgr;
    }
  return pcanvasNodeMgr;
}


canvasNode * canvasNodeMgr::add (uint32_t nodeId, uint32_t nodeSysId, qreal x, qreal y, QString nodeDescription)
{
  if (m_nodes.find (nodeId) != m_nodes.end ())
    {
      //NS_FATAL_ERROR ("NodeId:" << nodeId << " Already exists");
    }
  QPixmap pix (":/resources/ns3logo2.png","png");
  canvasNode * node = new canvasNode (nodeId, nodeSysId, x, y, nodeDescription);
  node->setPos (x, y);
  //node->setPixmap (pix);
  m_nodes[nodeId] = node;
  m_minX = qMin (m_minX, x);
  m_minY = qMin (m_minY, y);
  m_maxX = qMax (m_maxX, x);
  m_maxY = qMax (m_maxY, y);

  return node;
}


void
canvasNodeMgr::setSize (qreal width, qreal height)
{
  for (NodeIdcanvasNodeMap_t::const_iterator i = m_nodes.begin ();
      i != m_nodes.end ();
      ++i)
    {
      canvasNode * canvasNode = i->second;
      canvasNode->setSize (width, height);
    }
}

canvasNodeMgr::NodeIdcanvasNodeMap_t
canvasNodeMgr::getm_nodes()
{
    return m_nodes;
}

//void
//canvasNodeMgr::showRemainingBatteryCapacity (bool show)
//{
//  for (NodeIdcanvasNodeMap_t::const_iterator i = m_nodes.begin ();
//      i != m_nodes.end ();
//      ++i)
//    {
//      canvasNode * canvasNode = i->second;
//      canvasNode->updateBatteryCapacityImage (show);
//    }
//}

canvasNode *
canvasNodeMgr::getNode (uint32_t nodeId)
{
  return m_nodes[nodeId];
}

uint32_t
canvasNodeMgr::getCount ()
{
  return m_nodes.size ();
}


QPointF
canvasNodeMgr::getMinPoint ()
{
  return QPointF (m_minX, m_minY);
}

QPointF
canvasNodeMgr::getMaxPoint ()
{
  qreal m = qMax (m_maxX, m_maxY);
  return QPointF (m, m);
}


void
canvasNodeMgr::systemReset ()
{
  m_nodes.clear ();
  m_minX = 0;
  m_minY = 0;
  m_maxX = 0;
  m_maxY = 0;
  m_counterIdToNamesDouble.clear ();
  m_counterIdToNamesUint32.clear ();
}


void
canvasNodeMgr::addIpv4Address (uint32_t nodeId, QString ip)
{
  getNode (nodeId)->addIpv4Address (ip);
}

void
canvasNodeMgr::addMacAddress (uint32_t nodeId, QString mac)
{
  getNode (nodeId)->addMacAddress (mac);
}

void
canvasNodeMgr::showNodeId (bool show)
{
  for (NodeIdcanvasNodeMap_t::const_iterator i = m_nodes.begin ();
      i != m_nodes.end ();
      ++i)
    {
      canvasNode * canvasNode = i->second;
      canvasNode->showNodeId (show);
    }

}

void
canvasNodeMgr::showNodeSysId (bool show)
{
  for (NodeIdcanvasNodeMap_t::const_iterator i = m_nodes.begin ();
      i != m_nodes.end ();
      ++i)
    {
      canvasNode * canvasNode = i->second;
      canvasNode->showNodeSysId (show);
    }
}


canvasNodeMgr::TimePosVector_t
canvasNodeMgr::getPositions (uint32_t nodeId)
{
  return m_nodePositions[nodeId];
}

void
canvasNodeMgr::addAPosition (uint32_t nodeId, qreal t, QPointF pos)
{
  if (m_nodePositions.find (nodeId) == m_nodePositions.end ())
    {
      TimePosVector_t posVector;
      m_nodePositions[nodeId] = posVector;
    }
  TimePosVector_t & pv = m_nodePositions[nodeId];
  TimePosition_t tp;
  tp.p = pos;
  tp.t = t;
  pv.push_back (tp);
}


void
canvasNodeMgr::addNodeCounterUint32 (uint32_t counterId, QString counterName)
{
  m_counterIdToNamesUint32[counterId] = counterName;
}

void
canvasNodeMgr::addNodeCounterDouble (uint32_t counterId, QString counterName)
{
  m_counterIdToNamesDouble[counterId] = counterName;
}

void
canvasNodeMgr::updateNodeCounter (uint32_t nodeId, uint32_t counterId, qreal counterValue)
{
  canvasNode * canvasNode = getNode (nodeId);
  canvasNode::CounterType_t ct;
  bool counterFound = false;
  for (CounterIdName_t::const_iterator i = m_counterIdToNamesDouble.begin ();
       i != m_counterIdToNamesDouble.end ();
       ++i)
    {
      if (counterId == i->first)
        {
          ct = canvasNode::DOUBLE_COUNTER;
          counterFound = true;
          break;
        }
    }
  if (!counterFound)
    {
    for (CounterIdName_t::const_iterator i = m_counterIdToNamesUint32.begin ();
         i != m_counterIdToNamesUint32.end ();
         ++i)
      {
        if (counterId == i->first)
          {
            ct = canvasNode::UINT32_COUNTER;
            counterFound = true;
            break;
          }
      }
    }
  canvasNode->updateCounter (counterId, counterValue, ct);
}


canvasNodeMgr::CounterIdName_t
canvasNodeMgr::getUint32CounterNames ()
{
  return m_counterIdToNamesUint32;
}

canvasNodeMgr::CounterIdName_t
canvasNodeMgr::getDoubleCounterNames ()
{
  return m_counterIdToNamesDouble;
}



uint32_t
canvasNodeMgr::getCounterIdForName (QString counterName, bool &result, canvasNode::CounterType_t & counterType)
{
  result = false;
  for (CounterIdName_t::const_iterator i = m_counterIdToNamesDouble.begin ();
       i != m_counterIdToNamesDouble.end ();
       ++i)
    {
      QString n = i->second;
      if (n == counterName)
        {
          result = true;
          counterType = canvasNode::DOUBLE_COUNTER;
          return i->first;
        }
    }
  for (CounterIdName_t::const_iterator i = m_counterIdToNamesUint32.begin ();
       i != m_counterIdToNamesUint32.end ();
       ++i)
    {
      QString n = i->second;
      if (n == counterName)
        {
          result = true;
          counterType = canvasNode::UINT32_COUNTER;
          return i->first;
        }
    }
  return -1;
}



