#include "mainwindow.h"
#include <QApplication>
#include <unistd.h>
#include <canvasscene.h>
#include "canvasnodeinfo.h"
#include <modifyroutewidget.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
//    modifyRouteWidget v;
//    v.show();
    return a.exec();
}
