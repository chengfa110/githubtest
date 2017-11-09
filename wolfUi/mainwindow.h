#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include <thread>
using namespace std;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    int robotnumber;
    int checkBoxStates[10][2];
    void changeLocation();
    void startCommand();
    void endCommand();

private:
    QGroupBox *createParameterBox();
    QGroupBox *createloadBox();
    QGroupBox *createcommandBox();
    Ui::MainWindow *ui;
    QGroupBox *parameterBox;
    QGroupBox *loadBox;
    QGroupBox *commandBox;
    QLabel *robotNumberLaber;
    QLabel *robotlocationLabel;
    QLabel *robot0;
    QLabel *robot1;
    QLabel *robot2;
    QLabel *robot3;
    QLabel *robot4;
    QLabel *robot5;
    QLabel *robot6;
    QPushButton *parameterOk;
    QPushButton *locationChange;
    QPushButton *startCommandButton;
    QPushButton *endCommandButton;
    QPushButton *createLaunchButton;
    QPushButton *checkOk;
    QComboBox *robotNumberBox;
    QCheckBox *robot0Avoid;
    QCheckBox *robot1Action;
    QCheckBox *robot2Action;
    QCheckBox *robot3Action;
    QCheckBox *robot4Action;
    QCheckBox *robot5Action;
    QCheckBox *robot6Action;
    QCheckBox *robot1Distance;
    QCheckBox *robot2Distance;
    QCheckBox *robot3Distance;
    QCheckBox *robot4Distance;
    QCheckBox *robot5Distance;
    QCheckBox *robot6Distance;
    std::thread *newThread;

public slots:

    void createWorld();
    void getnumber(QString number);
    void showButtons();
    void createNewThread();
    void checkBoxChangeTest();
    void createLaunch();
    void createStartThread();
    void createEndThread();
};

#endif // MAINWINDOW_H
