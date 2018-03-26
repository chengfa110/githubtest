#include "lineitem.h"

LinkManager *pLinkManager=0;
lineitem::lineitem(uint32_t fromId, uint32_t toId):
    m_fromId(fromId),
    m_toId(toId)
{
    QLineF line (canvasNodeMgr::getInstance ()->getNode (m_fromId)->getCenter (),
                 canvasNodeMgr::getInstance ()->getNode (m_toId)->getCenter ());
    setLine (line);
}

void
lineitem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
//    QGraphicsLineItem::paint(painter,option,widget);

    QPen pen;
    pen.setCosmetic (true);
    QBrush brush;
    brush.setStyle (Qt::SolidPattern);
    brush.setColor (Qt::red);
    painter->setPen (pen);
    painter->setRenderHint(QPainter::Antialiasing, true);//设置反走样
    painter->setBrush(brush);
    painter->drawLine (line ());
}

void
lineitem::repairLink ()
{
  QLineF line (canvasNodeMgr::getInstance ()->getNode (m_fromId)->getCenter (),
               canvasNodeMgr::getInstance ()->getNode (m_toId)->getCenter ());
  setLine (line);
}

LinkManager::LinkManager ()
{

}

LinkManager *
LinkManager::getInstance ()
{
  if (!pLinkManager)
    {
      pLinkManager = new LinkManager;
    }
  return pLinkManager;
}

lineitem *
LinkManager::addLink (uint32_t fromId, uint32_t toId)
{
  lineitem * item = new lineitem (fromId,toId);
  if (m_pointToPointLinks.find (fromId) == m_pointToPointLinks.end ())
    {
      LinkManager::lineitemVector_t v;
      v.push_back (item);
      m_pointToPointLinks[fromId] = v;
      return item;
    }
  else
    {
      LinkManager::lineitemVector_t & v = m_pointToPointLinks[fromId];
      v.push_back (item);
      return item;
    }
}

LinkManager::NodeIdlineitemVectorMap_t *
LinkManager::getLinks ()
{
  return &m_pointToPointLinks;
}

lineitem *
LinkManager::getLineitem (uint32_t fromId, uint32_t toId)
{
  lineitem * theLink = 0;
  for (LinkManager::NodeIdlineitemVectorMap_t::const_iterator i = m_pointToPointLinks.begin ();
      i != m_pointToPointLinks.end ();
      ++i)
    {
      if (fromId != i->first)
        {
          continue;
        }
      LinkManager::lineitemVector_t v = i->second;
      for (LinkManager::lineitemVector_t::const_iterator j = v.begin ();
          j != v.end ();
          ++j)
        {
          lineitem * link = *j;
          if ( (link->m_fromId == fromId && link->m_toId == toId) ||
              (link->m_fromId == toId && link->m_toId == fromId))
            return link;
        }
    }
  return theLink;
}

void
LinkManager::updateLink (uint32_t fromId, uint32_t toId)
{
  lineitem * lineitem = getLineitem (fromId, toId);
  //animLink-
}

void
LinkManager::systemReset ()
{
  // remove links
  m_pointToPointLinks.clear ();

}

void
LinkManager::repairLinks (uint32_t nodeId)
{
  for (LinkManager::NodeIdlineitemVectorMap_t::const_iterator i = m_pointToPointLinks.begin ();
      i != m_pointToPointLinks.end ();
      ++i)
    {
      LinkManager::lineitemVector_t v = i->second;
      for (LinkManager::lineitemVector_t::const_iterator j = v.begin ();
          j != v.end ();
          ++j)
        {
          lineitem * animLink = *j;
          if ((animLink->m_fromId == nodeId) || (animLink->m_toId == nodeId))
            {
              animLink->repairLink ();
            }
        }
    }
}
