#ifndef LINEITEM_H
#define LINEITEM_H

#include <QtWidgets>
#include <canvasnode.h>

class lineitem : public QGraphicsLineItem
{
public:
    lineitem(uint32_t fromId, uint32_t toId);
    void paint (QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
    uint32_t m_fromId;
    uint32_t m_toId;
    void repairLink();

};

class LinkManager
{
public:
  typedef QVector <lineitem *> lineitemVector_t;
  typedef std::map <uint32_t, lineitemVector_t> NodeIdlineitemVectorMap_t;
  static LinkManager * getInstance ();
  lineitem * addLink (uint32_t fromId, uint32_t toId);

  NodeIdlineitemVectorMap_t * getLinks ();
  lineitem * getLineitem (uint32_t fromId, uint32_t toId);
  void updateLink (uint32_t fromId, uint32_t toId);
  void repairLinks (uint32_t nodeId);
  void systemReset ();

private:
  LinkManager ();
  //AnimLinkVector_t             m_pointToPointLinks;
  NodeIdlineitemVectorMap_t    m_pointToPointLinks;

};

#endif // LINEITEM_H
