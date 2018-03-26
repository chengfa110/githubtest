#include "routewindow.h"
#include "mainwindow.h"
#include <unistd.h>

RouteWindow * pRouteWindow = 0;
RouteWindow::RouteWindow(QWidget *parent):QWidget(parent)
{
    checkRouteThread();
    routeView = new QGraphicsView();
    routeView->setScene(RoutingScene::getInstance());
    QHBoxLayout *mainLayout =new QHBoxLayout(this);
    mainLayout->addWidget(routeView);
    resize(1000,1000);
}

void
RouteWindow::checkRoute()
{
    //使用ansible命令查询路由表并生成log文件
    QString ansible = QString("ansible group1 -m shell -a \"rm /home/ubuntu/route.log\"");
    QProcess::execute(ansible);
    ansible = QString("ansible group1 -m shell -a \"route -n>>/home/ubuntu/route.log\"");
    QProcess::execute(ansible);
//    std::cout<<"check route Success"<<std::endl;
    returnLog();
}

void
RouteWindow::returnLog()
{
    //使用脚本回传log文件
    system("ansible-playbook ~/Script/route.yaml");

}

void
RouteWindow::checkRouteThread()
{
    routeThread = new std::thread(&RouteWindow::checkRoute,this);
    routeThread->detach();
}

RouteWindow *
RouteWindow::getInstance()
{
    if (!pRouteWindow)
      {
        pRouteWindow = new RouteWindow;
      }
    return pRouteWindow;
}
