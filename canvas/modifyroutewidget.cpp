#include "modifyroutewidget.h"
#include <canvasscene.h>
#include <mainwindow.h>

modifyRouteWidget *pModifyRouteWidget = 0;

modifyRouteWidget::modifyRouteWidget(QWidget *parent): QWidget(parent)
{
    setWindowTitle("拓扑设置");
    createInterface();
    timer = new QTimer(this);
    timer->isSingleShot();//设置定时器为单次触发
    connect(timer, SIGNAL(timeout()), this, SLOT(handleTimeout()));
    Qt::WindowFlags flags = 0;
    setWindowFlags(flags);//禁止窗口最大化
    setFixedSize(230,180);//禁止改变窗口大小
}

void
modifyRouteWidget::createInterface()
{
    declarationLabel = new QLabel;
    QPalette pa;//设置declarationLabel字体
    pa.setColor(QPalette::WindowText,Qt::red);
    declarationLabel->setPalette(pa);
    declarationLabel->setMaximumHeight(20);
    fromIdLabel = new QLabel("fromID:");
    toIdLabel = new QLabel("   toID: ");
    QValidator *validator=new QIntValidator(0,100,this);
    fromIdLine = new QLineEdit;
    fromIdLine->setMaximumWidth(75);
    fromIdLine->setValidator(validator);//限制输入为0到100的int类型
    toIdLine = new QLineEdit;
    toIdLine->setMaximumWidth(75);
    toIdLine->setValidator(validator);//限制输入为0到100的int类型
    addRouteButton = new QPushButton("增加路由");
    addRouteButton->setMaximumWidth(100);
    connect(addRouteButton,SIGNAL(clicked(bool)),this,SLOT(addRouteSlot()));
    deleteRouteButton = new QPushButton("删除路由");
    deleteRouteButton->setMaximumWidth(100);
    connect(deleteRouteButton,SIGNAL(clicked(bool)),this,SLOT(deleteRouteSlot()));
    QGridLayout *grid = new QGridLayout;
    grid->addWidget(fromIdLabel,2,2);
    grid->addWidget(fromIdLine,2,3);
    grid->addWidget(toIdLabel,3,2);
    grid->addWidget(toIdLine,3,3);
    grid->addWidget(declarationLabel,4,2);
    grid->addWidget(addRouteButton,5,2);
    grid->addWidget(deleteRouteButton,5,3);
    setLayout(grid);
}

void
modifyRouteWidget::addRouteSlot()
{
    if(fromIdLine->text().toInt()>=0 && fromIdLine->text().toInt()<nodeCount && toIdLine->text().toInt()>=0 && toIdLine->text().toInt()<nodeCount)
    {
        QString fromIdString = fromIdLine->text();
        QString toIdString = toIdLine->text();
        if(fromIdString.isEmpty()||toIdString.isEmpty())
        {
             QMessageBox::warning(this,"警告","输入不能为空");
        }
        else
        {
            declarationLabel->setText("修改中，请稍后");
            canvasscene::getInstance()->addLineVector(fromIdString.toInt(),toIdString.toInt());
            //这里添加ansible命令控制容器删除路由规则
            createAddRouteThread();
        }
    }
    else
    {
        declarationLabel->setText("输入有误，请检查后修改");
    }
}

void
modifyRouteWidget::deleteRouteSlot()
{
    if(fromIdLine->text().toInt()>=0 && fromIdLine->text().toInt()<nodeCount && toIdLine->text().toInt()>=0 && toIdLine->text().toInt()<nodeCount)
    {
        QString fromIdString = fromIdLine->text();
        QString toIdString = toIdLine->text();
        if(fromIdString.isEmpty()||toIdString.isEmpty())
        {
            QMessageBox::warning(this,"警告","输入不能为空");
        }
        else
        {
            declarationLabel->setText("修改中，请稍后");
            canvasscene::getInstance()->deleteLineVector(fromIdString.toInt(),toIdString.toInt());
            //这里添加ansible命令控制容器增加路由规则
            createDeleteRouteThread();
        }
    }
    else
    {
        declarationLabel->setText("输入有误，请检查后修改");
    }
}

void
modifyRouteWidget::createAddRouteThread()
{
    addRouteButton->setEnabled(false);
    deleteRouteButton->setEnabled(false);
    timer->start(2500);
    modifyRouteThread = new std::thread(&modifyRouteWidget::addRouteAction,this);
    modifyRouteThread->detach();
}

void
modifyRouteWidget::createDeleteRouteThread()
{
    addRouteButton->setEnabled(false);
    deleteRouteButton->setEnabled(false);
    timer->start(2500);
    modifyRouteThread = new std::thread(&modifyRouteWidget::deleteRouteAction,this);
    modifyRouteThread->detach();
}

void
modifyRouteWidget::addRouteAction()
{
    int fromId,toId;
    if(fromIdLine->text().toInt() < toIdLine->text().toInt())
    {
        fromId = fromIdLine->text().toInt();
        toId = toIdLine->text().toInt();
    }
    else {
        fromId = toIdLine->text().toInt();
        toId = fromIdLine->text().toInt();
    }
    QString mac = canvasscene::getInstance()->getNodeInfo().at(toId).value(2);
    QString command = QString("ansible node%1 -m shell -a \"iptables -A INPUT -m mac --mac-source %2 -j DROP\"").arg(fromId).arg(mac);
    QProcess::execute(command);
}

void
modifyRouteWidget::deleteRouteAction()
{
    int fromId,toId;
    qDebug()<<nodeCount;
//    if(fromIdLine->text().toInt()>=0 && fromIdLine->text().toInt()<nodeCount && toIdLine->text().toInt()>=0 && toIdLine->text().toInt()<nodeCount)
//    {
        if(fromIdLine->text().toInt() < toIdLine->text().toInt())
        {
          fromId = fromIdLine->text().toInt();
          toId = toIdLine->text().toInt();
          }
         else {
           fromId = toIdLine->text().toInt();
            toId = fromIdLine->text().toInt();
        }
//    }
//    else
//    {
//        declarationLabel->setText("输入有误");
//    }
    QString mac = canvasscene::getInstance()->getNodeInfo().at(toId).value(2);
    QString command = QString("ansible node%1 -m shell -a \"iptables -D INPUT -m mac --mac-source %2 -j DROP\"").arg(fromId).arg(mac);
    QProcess::execute(command);
}

void
modifyRouteWidget::handleTimeout()
{
    addRouteButton->setEnabled(true);
    deleteRouteButton->setEnabled(true);
    declarationLabel->setText("完成，请继续");
}

modifyRouteWidget*
modifyRouteWidget::getInstance()
{
    if(!pModifyRouteWidget)
    {
        pModifyRouteWidget = new modifyRouteWidget;
    }
    return pModifyRouteWidget;
}

void
modifyRouteWidget::changeNodeCount(int x)
{
    nodeCount = x;
}
