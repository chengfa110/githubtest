#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include <thread>
#include <QDialog>
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
    void actionCommand();
    void attactCommand();

    QString qss1="QGroupBox {\
    border: 3px solid #2B2B2B;\
    border-radius: 4px;\
    margin-top: 2ex; \
    font-family:仿宋;\
    font:blod 18px;\
    }\
    QGroupBox::title {\
    subcontrol-origin: margin;\
    subcontrol-position: top left;\
    padding: 0 4px;\
    }";

private:
    QDialog *command;
    QGroupBox *createParameterBox();
    QGroupBox *createloadBox();
    QGroupBox *createcommandBox();
    Ui::MainWindow *ui;
    QGroupBox *parameterBox;
    QGroupBox *loadBox;
    QGroupBox *commandBox;
    QLabel *robotNumberLaber;
    QLabel *robotNumberLaber1;
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
    QPushButton *openWorldButton;
    QPushButton *openLaunchButton;
    QPushButton *actionButton;
    QPushButton *attactButton;
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
    std::thread *newThread1;

public slots:

    void createWorld();
    void getnumber(QString number);
    void showButtons();
    void createNewThread();
    void checkBoxChangeTest();
    void createLaunch();
    void createStartThread();
    void createEndThread();
    void createActionThread();
    void createAttactThread();
    void openWorld();
    void openLaunch();
    void clickedShowCommand();
    void clickedHideCommand();
};

#endif // MAINWINDOW_H
