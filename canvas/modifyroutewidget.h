#ifndef MODIFYROUTEWIDGET_H
#define MODIFYROUTEWIDGET_H

#include <QtWidgets>
#include <thread>

struct arc {
    int fromId;
    int toId;
};

class modifyRouteWidget : public QWidget
{
    Q_OBJECT
public:
    modifyRouteWidget(QWidget *parent = 0);
    void createInterface();
    void addRouteAction();
    void deleteRouteAction();
    void createAddRouteThread();
    void createDeleteRouteThread();
    static modifyRouteWidget* getInstance();
    int nodeCount;
    void changeNodeCount(int x);

private:
    QLabel *fromIdLabel;
    QLabel *toIdLabel;
    QLabel *declarationLabel;
    QLineEdit *fromIdLine;
    QLineEdit *toIdLine;
    QPushButton *addRouteButton;
    QPushButton *deleteRouteButton;
    std::thread *modifyRouteThread;
    QTimer * timer;

public slots:
    void addRouteSlot();
    void deleteRouteSlot();
    void handleTimeout();
};

#endif // MODIFYROUTEWIDGET_H
