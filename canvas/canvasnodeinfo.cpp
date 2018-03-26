#include "canvasnodeinfo.h"
#include <QItemSelectionModel>
#include <mainwindow.h>
#include <QFile>

canvasNodeInfo * pCanvasNodeInfo = 0;
canvasNodeInfo::canvasNodeInfo(QWidget *parent): QWidget(parent),
  lastCount(0)
{
    setWindowTitle(tr("节点配置"));
    resize(800,600);
}

void
canvasNodeInfo::setupModel(int nodecount)
{   

    if(lastCount != 0)
    {
//        for(int j=0;j<lastCount;j++)
//        model->removeRow(j,QModelIndex());
        model->setRowCount(nodecount);
    }
    else
    {
        model = new QStandardItemModel(nodecount,6,this);
        model->setHeaderData(0,Qt::Horizontal,tr("节点Id"));
        model->setHeaderData(1,Qt::Horizontal,tr("节点名称"));
        model->setHeaderData(2,Qt::Horizontal,tr("ipv4"));
        model->setHeaderData(3,Qt::Horizontal,tr("mac"));
        model->setHeaderData(4,Qt::Horizontal,tr("初始位置X"));
        model->setHeaderData(5,Qt::Horizontal,tr("初始位置Y"));
    }

        for(int i = 0 ; i < nodecount ; i++)
        {
            model->setData(model->index(i, 0, QModelIndex()),
              i);
            model->setData(model->index(i, 1, QModelIndex()),
              canvasscene::getInstance()->getNodeInfo().at(i).value(0));
            model->setData(model->index(i, 2, QModelIndex()),
              canvasscene::getInstance()->getNodeInfo().at(i).value(1));
            model->setData(model->index(i,3, QModelIndex()),
              canvasscene::getInstance()->getNodeInfo().at(i).value(2));
            model->setData(model->index(i,4, QModelIndex()),
              canvasscene::getInstance()->getInitialPosition(i).x());
            model->setData(model->index(i,5, QModelIndex()),
              canvasscene::getInstance()->getInitialPosition(i).y());
        }
        lastCount = nodecount;
    setupView();
    connect(model,SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)),this,SLOT(datachanged(QModelIndex)));
}

void
canvasNodeInfo::setupView()
{
    table = new QTableView;			//新建一个QTableView对象
    table->setModel(model);			//为QTableView对象设置相同的Model
    QItemSelectionModel *selectionModel=new QItemSelectionModel(model);
                                    //(a)
    table->setColumnWidth(3,160);
    table->setSelectionModel(selectionModel);
//    connect(selectionModel,SIGNAL(selectionChanged(QItemSelection, ItemSelection)),table,SLOT(selectionChanged(QItemSelection,QItemSelection)));									//(b)
    splitter = new QSplitter;
    splitter->setOrientation(Qt::Vertical);
    splitter->addWidget(table);

    QHBoxLayout *mainLayout =new QHBoxLayout(this);
    mainLayout->addWidget(splitter);
}

void
canvasNodeInfo::readFile()
{
    //该函数被canvassence中readfile()函数代替
    QFile file("home/zhang/canvas/IP_and_MAC.txt");
    if(file.open((QIODevice::ReadOnly|QIODevice::Text)))
    {
        QTextStream stream(&file);
        QString line;
        model->removeRows(0,model->rowCount(QModelIndex()),
              QModelIndex());
        int row = 0;
        int id = 0;
    do
    {
        line = stream.readLine();
        if (!line.isEmpty())
        {
               model->insertRows(row, 1, QModelIndex());
               QStringList pieces = line.split(" ", QString
                   ::SkipEmptyParts);
               model->setData(model->index(row, 0, QModelIndex()),
                   id);
               model->setData(model->index(row, 1, QModelIndex()),
                   pieces.value(0));
               model->setData(model->index(row, 2, QModelIndex()),
                   pieces.value(1));
               model->setData(model->index(row,3, QModelIndex()),
                   pieces.value(2));
               row++;
               id++;
        }
    } while (!line.isEmpty());
        file.close();
     }
}

void canvasNodeInfo::datachanged(QModelIndex index)
{
    qreal data = model->data(index).toDouble();
    int column = index.column();
    int row = index.row();
    canvasscene::getInstance()->InitialPositionChange(row,column,data);
}

canvasNodeInfo* canvasNodeInfo::getInstance()
{
    if (!pCanvasNodeInfo)
      {
        pCanvasNodeInfo = new canvasNodeInfo;
      }
    return pCanvasNodeInfo;
}


void canvasNodeInfo::closeEvent(QCloseEvent *event)
{
//    MainWindow::getInstance()->changeStopCheckQActionStatus();
}
