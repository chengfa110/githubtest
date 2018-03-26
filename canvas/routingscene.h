#ifndef ROUTINGSCENE_H
#define ROUTINGSCENE_H
#include <QtWidgets>
#include <QGraphicsScene>
#include <canvasconstants.h>

class RoutingScene: public QGraphicsScene
{
public:
    typedef std::map <uint32_t, QString> NodeNameMap;
    RoutingScene();
    static RoutingScene * getInstance ();
    void addProxyWidgets(NodeNameMap nodeNameMap);
    void adjustRect();
    QString parseXML(QString nodeName);

private:
    qreal m_lastX;
    qreal m_lastY;
    qreal m_bottomY;

};

#endif // ROUTINGSCENE_H
