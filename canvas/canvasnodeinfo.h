#ifndef CANVASNODEINFO_H
#define CANVASNODEINFO_H

#include <QAbstractTableModel>
#include <QVector>
#include <QMap>
#include <QTableView>
#include <QtWidgets>
#include <QCloseEvent>

class canvasNodeInfo : public QWidget
{
    Q_OBJECT
public:
    canvasNodeInfo(QWidget *parent = 0);
    void setupModel(int nodecount);
    void setupView();
    void readFile();
    static canvasNodeInfo* getInstance();

private:
    QStandardItemModel *model;
    QTableView *table;
    QAbstractItemView *histogram;
    QSplitter *splitter;
    int lastCount;


protected:
     void closeEvent(QCloseEvent *event);

public slots:
    void datachanged(QModelIndex index);

};

#endif // CANVASNODEINFO_H
