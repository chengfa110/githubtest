#ifndef CANVASSCENE_H
#define CANVASSCENE_H
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QtWidgets>
#include <canvasnode.h>
#include <dashed.h>
#include <lineitem.h>

extern QVector<canvasNode *> m_canvasNodes;
class canvasscene : public QGraphicsScene
{
    Q_OBJECT
public:
    canvasscene();
    void addGrid ();
    void addNode (canvasNode *canvasNode);
    static canvasscene * getInstance ();
    void setNodePos (canvasNode *animNode, qreal x, qreal y);
    void setShowNodeTrajectory (canvasNode *animNode);
    QVector<canvasNode *> getCanvasNodes();
    void removeNodes(int count);
    void readFile();
    QVector <QStringList> getNodeInfo();
    QPointF getInitialPosition(int i);
    void InitialPositionChange(int row, int column, qreal data);
    void InitializationLineVector (int count);
    void addLineVector(int x, int y);
    void deleteLineVector(int x, int y);
    void resetLinePos(int count);
    void drawLine(int count);
    void deleteLine(int count);
    void showLine(int count);
    QGraphicsLineItem *getLine(int id);

private:
    typedef QVector <QGraphicsSimpleTextItem*>     GridCoordinatesVector_t;
    typedef QVector <QGraphicsLineItem *>          LineItemVector_t;
    typedef QVector <QStringList> NodeInfo_t;
    int m_nGridLines;
    QLabel * m_mousePositionLabel;
    QGraphicsProxyWidget * m_mousePositionProxyWidget;
    LineItemVector_t             m_gridLines;
    LineItemVector_t   m_lineItems;
    GridCoordinatesVector_t      m_gridCoordinates;
    NodeInfo_t nodeInfo;
    QVector<canvasNode *>  m_canvasNodes;
    typedef QVector <QPointF> NodeinitialPos;
    NodeinitialPos nodeinitialPos;

    bool m_enableMousePositionLabel;
    bool lineVisiable;
    void markGridCoordinates ();
    void initGridCoordinates ();
    void setMousePositionLabel (QPointF pos);
    void mouseMoveEvent (QGraphicsSceneMouseEvent *event);
    void showMousePositionLabel (bool show);

    QVector <QVector <int> > LineVector;


public slots:
    void initialPosition(QString count);

};

#endif // CANVASSCENE_H
