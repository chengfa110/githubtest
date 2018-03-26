#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtWidgets>
#include <iostream>
#include <piperecevice.h>
#include <unistd.h>
#include <canvasnodeinfo.h>

MainWindow * pMainWindow = 0;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    nodeCount = 0;
    oldNodeCount = 0;
    createAction();
    createMenus();
    createToolBar();
//    createview();
    createNewThread();

    m_mainSplitter = new QSplitter;
    m_mainSplitter->addWidget(canvasview::getInstance());
    m_mainSplitter->addWidget (canvasPropertyBroswer::getInstance ());
    m_mainSplitter->setStretchFactor(0,80);
    m_mainSplitter->setStretchFactor(1,6);
    m_mainSplitter->setAutoFillBackground(true);

    m_vLayout = new QVBoxLayout;
    m_vLayout->setSpacing (0);
    m_vLayout->addWidget (m_mainSplitter);

    m_centralWidget = new QWidget;
    m_centralWidget->setLayout (m_vLayout);
    setCentralWidget(m_centralWidget);
    setWindowTitle ("NetAnim");

    setWindowState(Qt::WindowMaximized);
//    canvasPropertyBroswer::getInstance ()->postParse ();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void
MainWindow::showNodeInfo()
{
    if(nodeCount != 0)
    {
            nodeInfo = new canvasNodeInfo;
            nodeInfo->setupModel(nodeCount);
            nodeInfo->show();
            nodeInfo->setAttribute(Qt::WA_DeleteOnClose);
            connect(nodeInfo,SIGNAL(destroyed()),this,SLOT(changeStopCheckQActionStatus()));
            stopCheckQAction->setEnabled(false);
//        canvasNodeInfo::getInstance()->setupModel(nodeCount);
//        canvasNodeInfo::getInstance()->show();
    }
    else
    {
//        qDebug()<<"NO Node!!";
        QMessageBox::warning(this,"警告","请设置节点！！");
    }
}

//创建并返回视图
void
MainWindow::createview()
{
//    m_scene = new canvasscene;

    if(nodeCount > 0)
    {
        if(canvasscene::getInstance()->getCanvasNodes().empty())
        {
              for(uint32_t i=0;i<nodeCount;i++)
              {
                uint32_t nodeId=i;
                uint32_t nodeSysId=i;
                qreal x=canvasscene::getInstance()->getInitialPosition(i).x();
                qreal y=canvasscene::getInstance()->getInitialPosition(i).y();
                QString nodeDescription="";

                canvasNode *node = 0;

                node = canvasNodeMgr::getInstance ()->add (nodeId,
                                      nodeSysId,
                                      x,
                                      y,
                                      nodeDescription);
                node->setData(Qt::UserRole,i);
                node->addIpv4Address(canvasscene::getInstance()->getNodeInfo().at(i).value(1));
                node->addMacAddress(canvasscene::getInstance()->getNodeInfo().at(i).value(2));
                node->getDescription()->setPos(node->sceneBoundingRect ().bottomRight ());

                QString name = QString("node%1").arg(i+1);
                nodeNameMap[i] = name;
                canvasscene::getInstance()->addNode(node);
                canvasview::getInstance()->updateTransform();

              }
            canvasscene::getInstance()->addGrid();
            canvasscene::getInstance()->InitializationLineVector(nodeCount);
            canvasscene::getInstance()->drawLine(nodeCount);
            canvasPropertyBroswer::getInstance ()->postParse ();

        }
        else
        {
            for(uint32_t i=0;i<nodeCount;i++)
            {
                qreal x=canvasscene::getInstance()->getInitialPosition(i).x();
                qreal y=canvasscene::getInstance()->getInitialPosition(i).y();
                setNodePos(canvasscene::getInstance()->getCanvasNodes().value(i),x,y);
            }
    //        canvasscene::getInstance()->resetLinePos(nodeCount);

        }
    }
//    canvasview = new canvasview(canvasscene::getInstance());
//    m_view->updateTransform();

//    return m_view;
}

void
MainWindow::recevicePosition()
{
    pipeRecevice::getInstance()->recevicePosition();
}

//创建新的线程用以接收管道信息
void
MainWindow::createNewThread()
{
    newThread = new std::thread(&MainWindow::recevicePosition,this);
    newThread->detach();
}

void
MainWindow::createAction()
{
    modifyQAction = new QAction(tr("&配置网络"),this);
    modifyQAction->setStatusTip("修改LXC容器网络配置文件");
//    connect(modifyQAction,SIGNAL(triggered(bool)),this,SLOT(modifyConfiguration()));

    cloneQAction = new QAction(tr("&克隆容器"),this);
    cloneQAction->setStatusTip("克隆容器");
    connect(cloneQAction,SIGNAL(triggered(bool)),this,SLOT(clone()));

    startQAction = new QAction(tr("&启动脚本"),this);
    startQAction->setStatusTip("启动配置网桥和网卡的脚本");
    connect(startQAction,SIGNAL(triggered(bool)),this,SLOT(startSh()));

    destroyQAction = new QAction(tr("&删除容器"),this);
    destroyQAction->setStatusTip("删除容器");
    connect(destroyQAction,SIGNAL(triggered(bool)),this,SLOT(destroy()));

    startCheckRouteQAction = new QAction(tr("路由表查询"),this);
    startCheckRouteQAction->setIcon(QIcon(":/new/canvas/canvasResource/route.svg"));
    startCheckRouteQAction->setStatusTip("查询一次容器的路由表");
    startCheckRouteQAction->setEnabled(false);
    connect(startCheckRouteQAction,SIGNAL(triggered(bool)),this,SLOT(checkRouteOnce()));

    stopCheckQAction = new QAction(tr("配置节点信息"),this);
    stopCheckQAction->setIcon(QIcon(":/new/canvas/canvasResource/config.svg"));
    stopCheckQAction->setStatusTip("配置节点的名称等信息");
    connect(stopCheckQAction,SIGNAL(triggered(bool)),this,SLOT(showNodeInfo()));

    startButton = new QToolButton;
    startButton->setToolTip("start");
    startButton->setStatusTip("放置选择数量的节点（请先配置节点信息）");
    startButton->setIcon(QIcon(":/new/canvas/canvasResource/if_play_2628488.svg"));
    connect(startButton,SIGNAL(clicked(bool)),this,SLOT(createview()));
    connect(startButton,SIGNAL(clicked(bool)),this,SLOT(countBoxDisable()));

    hostNetwork = new QToolButton;
    hostNetwork->setToolTip("主机网络信息");
    hostNetwork->setStatusTip("获取主机ip地址以及网卡信息");
    hostNetwork->setIcon(QIcon(":/new/canvas/canvasResource/network.svg"));
    connect(hostNetwork,SIGNAL(clicked(bool)),this,SLOT(getHostNetwork()));

    linkButton = new QToolButton;
    linkButton->setIcon(QIcon(":/new/canvas/canvasResource/linkline.png"));
    linkButton->setStatusTip("隐藏或显示路由连线");
    linkButton->setCheckable(true);
    linkButton->setEnabled(false);
    connect(linkButton,SIGNAL(clicked()),this,SLOT(showLinkLine()));

    nodeCountLabel = new QLabel(tr("节点数量"));

    nodeCountBox = new QComboBox;
    QStringList nodeCount;

    nodeCount << "0"
              << "1"
              << "2"
              << "3"
              << "4"
              << "5"
              << "6"
              << "7"
              << "8"
              << "9"
              << "10"
              << "11"
              << "12"
              << "13"
              << "14"
              << "15"
              << "16"
              << "17"
              << "18"
              << "19"
              << "20";
    nodeCountBox->addItems(nodeCount);
    connect(nodeCountBox,SIGNAL(currentIndexChanged(QString)),this,SLOT(nodeCountChanged(QString)));
    connect(nodeCountBox,SIGNAL(currentIndexChanged(QString)),canvasscene::getInstance(),SLOT(initialPosition(QString)));

    modifyInterfaceButton = new QToolButton;
    modifyInterfaceButton->setIcon(QIcon(":/new/canvas/canvasResource/addordelete.png"));
    modifyInterfaceButton->setToolTip("修改拓扑配置");
    modifyInterfaceButton->setStatusTip("增加或删除容器间的路由规则");
    modifyInterfaceButton->setEnabled(false);
    connect(modifyInterfaceButton,SIGNAL(clicked(bool)),this,SLOT(showModifyInterface()));

    openFileQAction = new QAction(tr("节点配置"),this);
    connect(openFileQAction,SIGNAL(triggered(bool)),this,SLOT(openFileSlot()));

}

void
MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&文件"));
    fileMenu->addAction(openFileQAction);

    ConfigMenu = menuBar()->addMenu(tr("&配置工具"));
    ConfigMenu->addAction(modifyQAction);
    ConfigMenu->addAction(cloneQAction);
    ConfigMenu->addAction(destroyQAction);
    ConfigMenu->addAction(startQAction);
}

void
MainWindow::createToolBar()
{
    toolsBar = addToolBar(tr("&仿真工具"));

    toolsBar->addWidget(startButton);

    toolsBar->addWidget(nodeCountLabel);
    toolsBar->addWidget(nodeCountBox);
    toolsBar->addWidget(hostNetwork);     //主机网络

    toolsBar->addSeparator();
    toolsBar->addAction(stopCheckQAction);//配置节点信息
    toolsBar->addWidget(linkButton);      //显示连线
    toolsBar->addAction(startCheckRouteQAction); //查询路由
    toolsBar->addWidget(modifyInterfaceButton); //修改路由

}

//修改容器的网络配置文件（绝对路径要修改）
void
MainWindow::modifyConfiguration()
{
     int n;
     for(n=1;n<=2;n++)
     {
     QString strAll;
     QStringList strList;
     QFile readFile("/home/zhang/Script/config");
     if(readFile.open((QIODevice::ReadOnly|QIODevice::Text)))
     {
         QTextStream stream(&readFile);
         strAll=stream.readAll();
     }

     readFile.close();
     QFile writeFile("/home/zhang/Script/config");
     if(writeFile.open(QIODevice::WriteOnly|QIODevice::Text))
     {
             QTextStream stream(&writeFile);
             strList=strAll.split("\n");

             for(int i=0;i<strList.count();i++)
             {
                 if(i==strList.count()-1)
                 {
                     //最后一行不需要换行
                     stream<<strList.at(i);
                 }
                 else
                 {
                     stream<<strList.at(i)<<'\n';
                 }

                 if(strList.at(i).contains("#Modify1"))
                 {
                     qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));

                     QString content1 =QString("lxc.network.hwaddr = 1a:80:%1%2:%3%4:13:cd ").arg(qrand()%10).arg(qrand()%10).arg(qrand()%10).arg(qrand()%10);
                     QString content2 =QString("lxc.network.ipv4 = 10.0.3.%1/24").arg(n);
                     QString tempStr=strList.at(i+1);

                     tempStr.replace(0,tempStr.length(),content1);
                     stream<<tempStr<<'\n';
                     tempStr=strList.at(i+2);
                     tempStr.replace(0,tempStr.length(),content2);
                     stream<<tempStr<<'\n';
                     i+=2;
                 }
                 else if(strList.at(i).contains("#Modify2"))
                 {
                     qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
                     QString content1 =QString("lxc.network.link = br-%1").arg(n);
                     QString content2 =QString("lxc.network.hwaddr = 00:16:3e:8f:%1%2:%3%4 ").arg(qrand()%10).arg(qrand()%10).arg(qrand()%10).arg(qrand()%10);
                     QString content3 =QString("lxc.network.ipv4 = 10.0.0.%1/24").arg(n);
                     QString content4 =QString("lxc.utsname = node%1").arg(n);

                     QString tempStr=strList.at(i+1);
                     tempStr.replace(0,tempStr.length(),content1);
                     stream<<tempStr<<'\n';
                     tempStr=strList.at(i+2);
                     tempStr.replace(0,tempStr.length(),content2);
                     stream<<tempStr<<'\n';
                     tempStr=strList.at(i+3);
                     tempStr.replace(0,tempStr.length(),content3);
                     stream<<tempStr<<'\n';
                     tempStr=strList.at(i+4);
                     tempStr.replace(0,tempStr.length(),content4);
                     stream<<tempStr<<'\n';
                     i+=4;
                 }
             }
            }
     writeFile.close();
     QString content1 =QString("cp /home/zhang/Script/config /var/lib/lxc/node%1").arg(n);
     QProcess::execute(content1);
     std::cout<<"node"<<n<<"  Modify Success"<<std::endl;
    }
}

//克隆容器
void
MainWindow::clone()
{
    for(int i=31;i<=33;i++)
    {
        QString content=QString("sudo lxc-clone -o node1 -n node%1").arg(i);
        QProcess::execute(content);
        std::cout<<"node"<<i<<"  Clone Success"<<std::endl;
    }
}

//删除容器
void
MainWindow::destroy()
{
    for(int i=1;i<=3;i++)
    {
        QString content=QString("sudo lxc-stop  -n node%1").arg(i);
        QProcess::execute(content);
        content=QString("sudo lxc-destroy  -n node%1").arg(i);
        QProcess::execute(content);
        std::cout<<"node"<<i<<"  Destroy Success"<<std::endl;
    }
}

//自动生成创建桥和虚拟网卡的脚本并启动ns-3代码
void
MainWindow::startSh()
{
    QFile fileSh("/home/zhang/Script/startSh.sh");
    fileSh.open(QIODevice::ReadWrite);
    fileSh.resize(0);

    fileSh.write("#!/bin/bash\n");
    for(int i=1;i<=30;i++)
    {
        QString content = QString("sudo brctl addbr br-%1\nsudo tunctl -t tap-%2\nsudo ifconfig tap-%3 192.168.0.%4 promisc up\nsudo brctl addif br-%5 tap-%6\nsudo ifconfig br-%7 up\n").arg(i).arg(i).arg(i).arg(i).arg(i).arg(i).arg(i);
        fileSh.write(content.toLatin1(),content.length());
    }

    for(int i=1;i<=30;i++)
    {
         QString content = QString("sudo lxc-start -n node%1\n").arg(i);
          fileSh.write(content.toLatin1(),content.length());
    }
   // QString content = QString("cd /home/zhang/software/ns-allinone-3.25/ns-3.25/\n");
   //fileSh.write(content.toLatin1(),content.length());
    //content=QString("./waf --run udp_bc_wifi_virtual_machine");
    //fileSh.write(content.toLatin1(),content.length());
    QProcess::execute("sudo chmod +x /home/hpcl-1/source/startSh.sh");
    std::cout<<"Start.sh Create Success"<<std::endl;
}

//启动NS-3以及路由表论询
void
MainWindow::startNs3()
{
    system("cd ~/software/ns-allinone-3.25/ns-3.25");
    system("./waf --run tap-wifi-virtual-machine");
    std::cout<<"run ns-3 Success"<<std::endl;

//    timer->stop(20000);
}

//单击按钮查询所有容器路由表一次
void
MainWindow::checkRouteOnce()
{
    timer = new QTimer(this);
    timer->setSingleShot(true);
    connect(timer,SIGNAL(timeout()),this,SLOT(handleTimeOut()));
    timer->start(4000);//定时器每4秒更新一次路由表

    RoutingScene::getInstance()->clear();
    QGraphicsTextItem *text = new QGraphicsTextItem("正在查询，请稍后...");
    QFont font;
    font.setPointSize(30);
    font.setBold(true);
    text->setFont(font);
    RoutingScene::getInstance()->addItem(text);
    text->setPos(300,300);

    RouteWindow::getInstance()->show();

}


//停止论询并回传数据整合成xml文件
void
MainWindow::stopCheck()
{
    system("ansible-playbook ~/Script/route.yaml");
    routeXML();
}

void
MainWindow::routeXML()
{
    //读取每个节点的日志文件并修改格式
    for(int n=0 ; n<3 ; n++ )
    {
        double time=0.0;
        QString strAll;
        QStringList strList;
        QString fileName = QString("/home/zhang/Script/route/node%1_route.log").arg(n+1);
        QFile readFile(fileName);
        if(readFile.open((QIODevice::ReadOnly|QIODevice::Text)))
        {
            QTextStream stream(&readFile);
            strAll=stream.readAll();
        }

        readFile.close();
        QFile writeFile(fileName);
        if(writeFile.open(QIODevice::WriteOnly|QIODevice::Text))
        {
            QTextStream stream(&writeFile);
            strList=strAll.split("\n");
            QString firstLine = QString("<rt t=\"0\" id=\"%1\" info=\"Node: %2, Time: +0.0s, Local time: +0.0s, Ipv4ListRouting table").arg(n).arg(n);
            stream<<firstLine<<'\n';

            for(int i=1;i<strList.count();i++)
            {
                if(i==strList.count()-1)
                {
                    //最后一行不需要换行
                    stream<<strList.at(i);
                }
                else
                {
                    stream<<strList.at(i)<<'\n';
                }

                if(strList.at(i).contains("Kernel IP routing table")&&i==0)
                {
                    QString content =QString("<rt t=\"%1\" id=\"%2\" info=\"Node: %3, Time: +%4s, Local time: +%5s,Ipv4ListRouting table").arg(time).arg(n).arg(n).arg(time).arg(time);

                    stream<<content<<'\n';
                    time = time +5;
                }
                else if(strList.at(i).contains("Kernel IP routing table")&&i!=0)
                {
                    QString content =QString("\" />\n<rt t=\"%1\" id=\"%2\" info=\"Node: %3, Time: +%4s, Local time: +%5s,Ipv4ListRouting table").arg(time).arg(n).arg(n).arg(time).arg(time);

                    stream<<content<<'\n';

                    time = time +5;
                }
        }
            QString content =QString("\" />\n");
            stream<<content;
      }
        writeFile.close();
   }

    //合并文件成一个XML文件
    QFile writeFile1("/home/zhang/Script/route/route.xml");
    writeFile1.open(QIODevice::ReadWrite);
    writeFile1.resize(0);

        QString content = QString("<anim ver=\"netanim-3.106\" filetype=\"routing\" >\n");
        writeFile1.write(content.toLatin1(),content.length());

        for(int n=0 ; n<3 ; n++ )
        {
            QString fileName = QString("/home/zhang/Script/route/node%1_route.log").arg(n+1);
            QFile readfile1(fileName);
            readfile1.open(QIODevice::ReadOnly);

            while(!readfile1.atEnd())
            {
                QString content = readfile1.readLine();
                writeFile1.write(content.toLatin1(),content.length());
            }
        }
        QString content1 = QString("</anim>");
        writeFile1.write(content1.toLatin1(),content1.length());

        QString ansible = QString("ansible group1 -m shell -a \"rm /home/ubuntu/route.log\"");
        QProcess::execute(ansible);

}

void
MainWindow::nodeCountChanged(QString count)
{
    if(count.toInt() <= canvasscene::getInstance()->getNodeInfo().size())
        nodeCount = count.toInt();
    else
    {
        nodeCount = canvasscene::getInstance()->getNodeInfo().size();
        QString waring = QString("配置文件中仅有%1个节点").arg(canvasscene::getInstance()->getNodeInfo().size());
        QMessageBox::warning(this,"错误",waring);
    }
}

int
MainWindow::getNodeCount()
{
    return nodeCount;
}

void
MainWindow::countBoxDisable()
{
//    stopCheckQAction->setDisabled(true);
//    startButton->setDisabled(true);
    if(nodeCount > 0)
    {
        linkButton->setEnabled(true);
        nodeCountBox->setEnabled(true);
        modifyInterfaceButton->setEnabled(true);
        startCheckRouteQAction->setEnabled(true);
        canvasscene::getInstance()->setSceneRect (0, 0, ANIMATORSCENE_USERAREA_WIDTH, ANIMATORSCENE_USERAREA_WIDTH);
    }
}

//获取主机网络信息
void
MainWindow::getHostNetwork()
{
    QString detail="";
    detail = detail + "主机名:  " + QHostInfo::localHostName() + "\n" + "\n";

    QList<QNetworkInterface> list=QNetworkInterface::allInterfaces();

    qDebug() <<list.count();

    foreach (QNetworkInterface address, list) {
            qDebug() <<address;
    }


    for(int i=0;i<list.count();i++)
    {
        QNetworkInterface interface=list.at(i);
        detail=detail+tr("设备：")+interface.name()+"\n";

        detail=detail+tr("硬件地址：")+interface.hardwareAddress()+"\n";

        QList<QNetworkAddressEntry> entryList=interface.addressEntries();

        for(int j=0;j<entryList.count();j++)
        {
            QNetworkAddressEntry entry=entryList.at(j);
            detail=detail+"\t"+tr("IP 地址：")+entry.ip().toString()+"\n";
            detail=detail+"\t"+tr("子网掩码：")+entry.netmask().toString() +"\n";
           detail=detail+"\t"+tr("广播地址：")+entry.broadcast().toString() +"\n";
        }
    }
    QMessageBox::information(this,tr("Detail"),detail);
}

MainWindow *
MainWindow::getInstance()
{
    return pMainWindow;
}


void
MainWindow::setNodeResource (canvasNode *animNode, uint32_t resourceId)
{
//  animNode->setResource (resourceId);
//  LinkManager::getInstance ()->repairLinks (animNode->getNodeId ());
}

void
MainWindow::setNodeSize (canvasNode *animNode, qreal size)
{
  animNode->setSize (size, size);
  animNode->getDescription ()->setPos (animNode->sceneBoundingRect ().bottomRight ());
  LinkManager::getInstance ()->repairLinks (animNode->getNodeId ());

}

void
MainWindow::setNodePos (canvasNode *animNode, qreal x, qreal y)
{
  //NS_LOG_DEBUG ("SetNodePos:" << animNode->getNodeId ());
  animNode->setX (x);
  animNode->setY (y);
  animNode->setPos (x, y);
  animNode->getDescription ()->setPos (animNode->sceneBoundingRect ().bottomRight ());
  animNode->getDashed()->setPos(animNode->sceneBoundingRect().center());
  LinkManager::getInstance ()->repairLinks (animNode->getNodeId ());
}

void
MainWindow::setNodeSysId (canvasNode * animNode, uint32_t sysId)
{
  animNode->updateNodeSysId (sysId, animNode->isVisibleNodeSysId());
}

void
MainWindow::setShowNodeTrajectory (canvasNode *animNode)
{
  canvasscene::getInstance ()->setShowNodeTrajectory (animNode);
}

void
MainWindow::showLinkLine()
{
    if(linkButton->isChecked())
    {
        canvasscene::getInstance()->showLine(nodeCount);
        linkButton->setToolTip("隐藏路由连线");
    }
    else
    {
        canvasscene::getInstance()->deleteLine(nodeCount);
        linkButton->setToolTip("显示路由连线");
    }
}

void
MainWindow::showModifyInterface()
{
//        modifyRoutewidget = new modifyRouteWidget;
//        modifyRoutewidget->show();
    modifyRouteWidget::getInstance()->show();
    modifyRouteWidget::getInstance()->changeNodeCount(nodeCount);
}

void
MainWindow::handleTimeOut()
{
    RoutingScene::getInstance()->clear();
    RoutingScene::getInstance()->addProxyWidgets(getnodeNameMap());
    qDebug()<<"刷新刷新！！！";
}

void
MainWindow::openFileSlot()
{
    QString fileName = QFileDialog::getOpenFileName(
                    this, tr("open node file"),
                    "../canvas", tr("Node files(*.txt);;All files (*.*)"));

    if(fileName.isEmpty())
    {
        QMessageBox::warning(this,"警告","没有文件");
    }
    else
    {
        file = QString("gedit %1").arg(fileName);
        openFileThread();
    }

}

void
MainWindow::openFileThread()
{
    fileThread = new std::thread(&MainWindow::openFileExecute,this);
    fileThread->detach();
}

void
MainWindow::openFileExecute()
{
    QProcess::execute(file);
}

MainWindow::NodeNameMap MainWindow::getnodeNameMap()
{
    return nodeNameMap;
}

void
MainWindow::changeStopCheckQActionStatus()
{
    stopCheckQAction->setEnabled(true);
}
