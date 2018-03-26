#ifndef ROUTEWINDOW_H
#define ROUTEWINDOW_H
#include <QtWidgets>
#include <QGraphicsProxyWidget>
#include <QGraphicsScene>
#include <routingscene.h>
#include <thread>

class RouteWindow : public QWidget
{
    Q_OBJECT
public:
    RouteWindow(QWidget *parent = 0);
    void checkRoute();
    void returnLog();

    QGraphicsView *routeView ;
    QGraphicsScene *routeScene;
    static RouteWindow * getInstance();
private:
    std::thread *routeThread;

public slots:
    void checkRouteThread();
};

#endif // ROUTEWINDOW_H
