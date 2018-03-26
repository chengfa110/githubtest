#ifndef PIPERECEVICE_H
#define PIPERECEVICE_H
#include <QtWidgets>

typedef struct{
    double time;
    int nodeId;
    qreal x;
    qreal y;
}nodePosition;

typedef QVector<nodePosition> nodePosition_t;

class pipeRecevice
{
public:
    pipeRecevice();
    void recevicePosition();
    nodePosition getPosition();
    static pipeRecevice * getInstance();
    nodePosition position;
    int getflag();
    void setFlagZero();
    int flag = 0;
private:

};

#endif // PIPERECEVICE_H
