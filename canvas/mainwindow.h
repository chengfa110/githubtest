
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <piperecevice.h>
#include <thread>
#include <canvasnode.h>
#include <canvasview.h>
#include <canvasscene.h>
#include <canvasnode.h>
#include <canvaspropertybroswer.h>
#include <routewindow.h>
#include <QHostInfo>
#include <QNetworkInterface>
#include <canvasnodeinfo.h>
#include <modifyroutewidget.h>

class Spreadsheet;
//extern nodePosition position;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    typedef std::map <uint32_t, QString> NodeNameMap;
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void createNewThread();
    void recevicePosition();
    void setNodeSize (canvasNode * animNode, qreal size);
    void setNodePos (canvasNode * animNode, qreal x, qreal y);
    void setNodeResource (canvasNode * animNode, uint32_t resourceId);
    void setNodeSysId (canvasNode * animNode, uint32_t sysId);
    void startNs3();
    void checkRoute();
    void routeXML();
    int getNodeCount();
    static MainWindow * getInstance();
    void setShowNodeTrajectory (canvasNode *animNode);
    NodeNameMap nodeNameMap;
    NodeNameMap getnodeNameMap();


    int nodeCount;
    int oldNodeCount;

private:
    typedef QVector <QGraphicsEllipseItem *>          EllipesItemVector_t;
    Ui::MainWindow *ui;
    QSplitter * m_mainSplitter;
    QVBoxLayout * m_vLayout;
    QWidget * m_centralWidget;
//    canvasscene *m_scene;
//    canvasview *m_view;
    std::thread *newThread;
    std::thread *fileThread;
    EllipesItemVector_t    m_Ellipesnode;
    QTimer *timer;
    QString file;


    //控件和功能模块
    void createAction();
    void createMenus();
    void createToolBar();
    void createCanvas();
    void openFileThread();
    void openFileExecute();
    void changePosition(QPointF pointF);


    QMenu *ConfigMenu;
    QMenu *fileMenu;
    QToolBar *toolsBar;
    QAction *modifyQAction;
    QAction *cloneQAction;
    QAction *destroyQAction;
    QAction *startQAction;
    QAction *startCheckRouteQAction;
    QAction *stopCheckQAction;
    QAction *openFileQAction;
    QToolButton *startButton;
    QComboBox *nodeCountBox;
    QLabel *nodeCountLabel;
    QToolButton *hostNetwork;
    QToolButton *linkButton;
    QToolButton *modifyInterfaceButton;

    canvasNodeInfo * nodeInfo;
    modifyRouteWidget * modifyRoutewidget;

public slots:
    void modifyConfiguration();
    void clone();
    void destroy();
    void startSh();
    void stopCheck();
    void checkRouteOnce();
    void nodeCountChanged(QString count);
    void createview();
    void countBoxDisable();
    void getHostNetwork();
    void showNodeInfo();
    void showLinkLine();
    void showModifyInterface();
    void handleTimeOut();
    void openFileSlot();
    void changeStopCheckQActionStatus();
};

#endif // MAINWINDOW_H
