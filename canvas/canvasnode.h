#ifndef CANVASNODE_H
#define CANVASNODE_H
#include <QGraphicsEllipseItem>
#include<QtWidgets>
#include "resizeableitem.h"
#include "common.h"
#include "dashed.h"

typedef
struct {
  qreal t;
  QPointF p;
} TimePosition_t;

class canvasNode: public ResizeableItem
{
public:
  typedef QVector <QString> Ipv4Vector_t;
  typedef QVector <QString> MacVector_t;
  typedef std::map <uint32_t, uint32_t> CounterIdValueUint32_t;
  typedef std::map <uint32_t, double> CounterIdValueDouble_t;

  typedef enum {
    UINT32_COUNTER,
    DOUBLE_COUNTER
  } CounterType_t;

  canvasNode (uint32_t nodeId, uint32_t nodeSysId, qreal x, qreal y, QString nodeDescription);
  ~canvasNode ();
  void paint (QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
  void mouseMoveEvent (QGraphicsSceneMouseEvent *event);
  void setNodeDescription (QString description);
  QPointF getCenter ();
  QGraphicsTextItem * getDescription ();
  Dashed * getDashed();
  qreal getX ();
  qreal getY ();
  void setX (qreal x);
  void setY (qreal y);
  bool getShowNodeTrajectory ();
  QColor getColor ();
  uint32_t getNodeId ();
  uint32_t getNodeSysId ();
  qreal getWidth ();
  int getResourceId ();
  Ipv4Vector_t getIpv4Addresses ();
  MacVector_t getMacAddresses ();
  void setWidth (qreal width);
  void setHeight (qreal height);
  void setColor (uint8_t r, uint8_t g, uint8_t b, uint8_t alpha = 255);
//  void setResource (int resourceId);
  void setPos (qreal x, qreal y);
  void setShowNodeTrajectory (bool showNodeTrajectory);
  void addIpv4Address (QString ip);
  void addMacAddress (QString mac);
  bool hasIpv4 (QString ip);
  bool hasMac (QString mac);
  void showNodeId (bool show);
  void showNodeSysId (bool show);
  bool isVisibleNodeSysId () const;
  void updateCounter (uint32_t counterId, qreal counterValue, CounterType_t counterType);

  qreal getDoubleCounterValue (uint32_t counterId, bool & result);
  uint32_t getUint32CounterValue (uint32_t counterId, bool & result);
//  void updateBatteryCapacityImage (bool show);
  void updateNodeSysId (uint32_t nodeSysId, bool show);

private:
  Dashed * m_dashed;
  QGraphicsTextItem * m_nodeDescription;
  uint32_t m_nodeId;
  uint32_t m_nodeSysId;
  qreal m_x;
  qreal m_y;
  bool m_showNodeId;
  bool m_showNodeSysId;
  Ipv4Vector_t m_ipv4Vector;
  MacVector_t m_macVector;
  int m_resourceId;
  bool m_showNodeTrajectory;
  QPixmap m_batteryPixmap; //!< Battery image
  bool m_showBatteryCapcity;

  QColor m_lastColor;

  CounterIdValueUint32_t m_counterIdToValuesUint32;
  CounterIdValueDouble_t m_counterIdToValuesDouble;

};


class canvasNodeMgr
{
public:
  typedef std::map <uint32_t, canvasNode *> NodeIdcanvasNodeMap_t;
  typedef QVector <TimePosition_t> TimePosVector_t;
  typedef std::map <uint32_t, TimePosVector_t> NodeIdPositionMap_t;
  typedef std::map <uint32_t, QString> CounterIdName_t;

  static canvasNodeMgr * getInstance ();
  canvasNode * getNode (uint32_t nodeId);
  canvasNode * add (uint32_t nodeId, uint32_t nodeSysId, qreal x, qreal y, QString nodeDescription);
  uint32_t getCount ();
  QPointF getMinPoint ();
  QPointF getMaxPoint ();
  void systemReset ();
  void addIpv4Address (uint32_t nodeId, QString ip);
  void addMacAddress (uint32_t nodeId, QString mac);
  void setSize (qreal width, qreal height);
  void showNodeId (bool show);
  void showNodeSysId (bool show);
  TimePosVector_t getPositions (uint32_t nodeId);
  void addAPosition (uint32_t nodeId, qreal t, QPointF pos);
//  void showRemainingBatteryCapacity (bool show);

  void addNodeCounterUint32 (uint32_t counterId, QString counterName);
  void addNodeCounterDouble (uint32_t counterId, QString counterName);
  void updateNodeCounter (uint32_t nodeId, uint32_t counterId, qreal counterValue);

  CounterIdName_t getUint32CounterNames ();
  CounterIdName_t getDoubleCounterNames ();
  NodeIdcanvasNodeMap_t getm_nodes();
  uint32_t getCounterIdForName (QString counterName, bool & result, canvasNode::CounterType_t & counterType);

private:
  canvasNodeMgr ();
  NodeIdcanvasNodeMap_t m_nodes;
  qreal m_minX;
  qreal m_minY;
  qreal m_maxX;
  qreal m_maxY;
  NodeIdPositionMap_t m_nodePositions;
  CounterIdName_t m_counterIdToNamesUint32;
  CounterIdName_t m_counterIdToNamesDouble;

};

#endif // CANVASNODE_H
