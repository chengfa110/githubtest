#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtWidgets>
#include <QFile>
#include <QDebug>
#include <QXmlStreamWriter>
#include <QProcess>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QGridLayout *grid = new QGridLayout;
    grid->addWidget(createParameterBox(),0,0);
    grid->addWidget(createloadBox(),1,0);
    grid->addWidget(createcommandBox(),2,0);

    QWidget *widget = new QWidget;
    widget->setLayout(grid);
    widget->resize(580, 320);
    widget->setMaximumSize(450,500);
    setCentralWidget(widget);
    setWindowTitle(tr("Group Boxes"));
}

MainWindow::~MainWindow()
{
    delete ui;
}

QGroupBox *MainWindow::createParameterBox()
{
    parameterBox = new QGroupBox(tr("第一步：设置参数"));
    parameterBox->setStyleSheet(qss1);
    robotNumberLaber = new QLabel(tr("机器人数量"));
    robotNumberLaber->setToolTip(tr("追击者数量"));
    robotlocationLabel = new QLabel(tr("机器人位置"));
    robotlocationLabel->setToolTip(tr("启动stage手动修改机器人位置"));
    parameterOk = new QPushButton(tr("确认"));
    locationChange = new QPushButton(tr("改变位置"));
    connect(locationChange,SIGNAL(clicked(bool)),this,SLOT(createNewThread()));
    robotNumberBox= new QComboBox();
    for(int i=4;i<=7;i++)
    {
        robotNumberBox->addItem(QString("%1").arg(i-1));
    }
    openWorldButton = new QPushButton(tr("打开world文件"));
    connect(openWorldButton,SIGNAL(clicked(bool)),this,SLOT(openWorld()));
    connect(robotNumberBox,SIGNAL(currentIndexChanged(QString)),this,SLOT(getnumber(QString)));
    connect(parameterOk,SIGNAL(clicked(bool)),this,SLOT(createWorld()));
    connect(parameterOk,SIGNAL(clicked(bool)),this,SLOT(showButtons()));
    QGridLayout *hbox = new QGridLayout;
    hbox->addWidget(robotNumberLaber,0,0);
    hbox->addWidget(robotNumberBox,0,1);
    hbox->addWidget(parameterOk,0,2);
    hbox->addWidget(robotlocationLabel,1,0);
    hbox->addWidget(locationChange,1,1);
    hbox->addWidget(openWorldButton,1,2);
    parameterBox->setLayout(hbox);

    return parameterBox;
}

QGroupBox *MainWindow::createloadBox()
{
    loadBox = new QGroupBox(tr("第二步：加载任务"));
    loadBox->setStyleSheet(qss1);
    createLaunchButton= new QPushButton(tr("创建Launch文件"));
    openLaunchButton = new QPushButton(tr("打开launch文件"));
    connect(openLaunchButton,SIGNAL(clicked(bool)),this,SLOT(openLaunch()));
    robot0 = new QLabel(tr("Goal"));
    robot0->setToolTip(tr("被追击目标"));
    robot1 = new QLabel(tr("robot-1"));
    robot2 = new QLabel(tr("robot-2"));
    robot3 = new QLabel(tr("robot-3"));
    robot4 = new QLabel(tr("robot-4"));
    robot5 = new QLabel(tr("robot-5"));
    robot6 = new QLabel(tr("robot-6"));
    QGridLayout *hbox = new QGridLayout;
    hbox->addWidget(robot0,0,0);
    hbox->addWidget(robot1,1,0);
    hbox->addWidget(robot2,2,0);
    hbox->addWidget(robot3,3,0);
    hbox->addWidget(robot4,4,0);
    hbox->addWidget(robot5,5,0);
    hbox->addWidget(robot6,6,0);
    robot0Avoid = new QCheckBox(tr("Avoid"));
    robot0Avoid->setCheckState(Qt::Checked);
    robot1Action = new QCheckBox(tr("Action"));
    robot1Action->setCheckState(Qt::Checked);
    robot2Action = new QCheckBox(tr("Action"));
    robot2Action->setCheckState(Qt::Checked);
    robot3Action = new QCheckBox(tr("Action"));
    robot3Action->setCheckState(Qt::Checked);
    robot4Action = new QCheckBox(tr("Action"));
    robot5Action = new QCheckBox(tr("Action"));
    robot6Action = new QCheckBox(tr("Action"));
    robot1Distance = new QCheckBox(tr("Distance"));
    robot1Distance->setCheckState(Qt::Checked);
    robot2Distance = new QCheckBox(tr("Distance"));
    robot2Distance->setCheckState(Qt::Checked);
    robot3Distance = new QCheckBox(tr("Distance"));
    robot3Distance->setCheckState(Qt::Checked);
    robot4Distance = new QCheckBox(tr("Distance"));
    robot5Distance = new QCheckBox(tr("Distance"));
    robot6Distance = new QCheckBox(tr("Distance"));
    checkOk = new QPushButton(tr("确认"));
    hbox->addWidget(robot0Avoid,0,1);
    hbox->addWidget(robot1Action,1,1);
    hbox->addWidget(robot2Action,2,1);
    hbox->addWidget(robot3Action,3,1);
    hbox->addWidget(robot4Action,4,1);
    hbox->addWidget(robot5Action,5,1);
    hbox->addWidget(robot6Action,6,1);
    hbox->addWidget(robot1Distance,1,2);
    hbox->addWidget(robot2Distance,2,2);
    hbox->addWidget(robot3Distance,3,2);
    hbox->addWidget(robot4Distance,4,2);
    hbox->addWidget(robot5Distance,5,2);
    hbox->addWidget(robot6Distance,6,2);
    hbox->addWidget(createLaunchButton,7,1);
    hbox->addWidget(checkOk,7,0);
    hbox->addWidget(openLaunchButton,7,2);
    connect(checkOk,SIGNAL(clicked(bool)),this,SLOT(checkBoxChangeTest()));
    connect(createLaunchButton,SIGNAL(clicked(bool)),this,SLOT(createLaunch()));
    robot4->hide();
    robot5->hide();
    robot6->hide();
    robot4Action->hide();
    robot5Action->hide();
    robot6Action->hide();
    robot4Distance->hide();
    robot5Distance->hide();
    robot6Distance->hide();
    loadBox->setLayout(hbox);

    return loadBox;
}

QGroupBox *MainWindow::createcommandBox()
{
    commandBox = new QGroupBox(tr("第三步：执行任务"));
    commandBox->setStyleSheet(qss1);
    startCommandButton = new QPushButton(tr("开始仿真"));
    endCommandButton = new QPushButton(tr("结束仿真"));
    connect(startCommandButton,SIGNAL(clicked(bool)),this,SLOT(createStartThread()));
    connect(endCommandButton,SIGNAL(clicked(bool)),this,SLOT(createEndThread()));
    QGridLayout *grid = new QGridLayout;
    grid->addWidget(startCommandButton,0,0);
    grid->addWidget(endCommandButton,1,0);
    commandBox->setLayout(grid);
    return commandBox;
}

void MainWindow::createNewThread()
{
    newThread = new std::thread(&MainWindow::changeLocation,this);
    newThread->detach();
}

void MainWindow::changeLocation()
{

    system("roslaunch wolf test.launch");
    //exit(0);
}

void MainWindow::createWorld()
{
    QFile file1("/home/zhang/catkin_ws/src/wolf/world/willow-erratic.world");
    file1.open(QIODevice::ReadOnly);
    QFile file2("/home/zhang/catkin_ws/src/wolf/world/new.world");
    file2.open(QIODevice::ReadWrite);
    file2.resize(0);

    while(!file1.atEnd())
    {
        QString content = file1.readLine();
        file2.write(content.toLatin1(),content.length());
    }

    if(robotnumber == 3)
        return;
    else if (robotnumber > 3)
    {
        for(int i=3;i<robotnumber;i++)
        {
            QString content = QString("erratic \n(\npose [%1 -7.667 5.000 0.000]\nname \"turtle%2").arg(i).arg(i);
            content +="\"\ncolor \"red\"\n)\n\n";
            file2.write(content.toLatin1(),content.length());
        }
    }
        return;
}

void MainWindow::createLaunch()
{
    QString strFile("/home/zhang/catkin_ws/src/wolf/launch/new.launch");
    QFile file(strFile);
    if (!file.open(QFile::WriteOnly | QFile::Text)) { // 只写模式打开文件
       qDebug() << QString("Cannot write file %1(%2).").arg(strFile).arg(file.errorString());
        return;
    }
    QXmlStreamWriter writer(&file);
    writer.setAutoFormatting(true);
    writer.writeStartElement("launch");
    writer.writeDTD("<param name=\"robot_goal\" value=\"robot_0/base_pose_ground_truth\"/>");
    writer.writeDTD("<node name = \"stageros\" pkg=\"stage_ros\" type = \"stageros\" args=\"$(find wolf)/world/new.world\" />");
    writer.writeDTD("<node name=\"map_server\" pkg=\"map_server\" type=\"map_server\" args=\"$(find wolf)/world/tutorial400.yaml\" respawn=\"false\"> ");
    writer.writeDTD("<param name=\"frame_id\" value=\"/map\" />");
    writer.writeDTD("</node>");

    writer.writeDTD("<!--robot_0-->");
    writer.writeDTD("<group ns=\"robot_0\">");
    writer.writeDTD("<param name=\"tf_prefix\" value=\"robot_0\" />");
    writer.writeDTD("<param name=\"robot_id\" value=\"0\"/>");
    if(checkBoxStates[0][0] == 1)
        writer.writeDTD("<node name=\"avoid\" pkg=\"wolf\" type=\"avoid\" respawn=\"false\" output=\"screen\"/>");
    writer.writeDTD("</group>");

    for(int i=1;i<=robotnumber;i++)
    {
        QString new1 = QString("<!--robot_%1-->").arg(i);
        writer.writeDTD(new1);
        new1 = QString("<group ns=\"robot_%1\">").arg(i);
        writer.writeDTD(new1);
        new1 = QString("<param name=\"tf_prefix\" value=\"robot_%1\" />").arg(i);
        writer.writeDTD(new1);
        new1 = QString("<param name=\"robot_id\" type=\"int\"  value=\"%1\"/>").arg(i);
        writer.writeDTD(new1);
        if(checkBoxStates[i][1] == 1)
        {
            new1 = QString("<node name=\"Distance%1\" pkg=\"wolf\" type=\"Distance\" respawn=\"false\" output=\"screen\">").arg(i);
            writer.writeDTD(new1);
            new1 = QString("<param name=\"robot_this\" value=\"robot_%1/base_pose_ground_truth\"/>").arg(i);
            writer.writeDTD(new1);
            writer.writeDTD("</node>");
        }
        if(checkBoxStates[i][0] == 1)
        {
            new1 =QString("<node name=\"action%1\" pkg=\"wolf\" type=\"action\" respawn=\"false\" output=\"screen\"/>").arg(i);
            writer.writeDTD(new1);
        }
        writer.writeDTD("</group>");
    }
    writer.writeDTD("<node name=\"decide\" pkg=\"wolf\" type=\"decide\" respawn=\"false\" output=\"screen\"/>");
    writer.writeEndElement();
}

void MainWindow::checkBoxChangeTest()
{
    if(robot0Avoid->checkState() == Qt::Checked)
        checkBoxStates[0][0]=1;

    if(robot1Action->checkState() == Qt::Checked)
        checkBoxStates[1][0]=1;
    if(robot1Distance->checkState() == Qt::Checked)
        checkBoxStates[1][1]=1;

    if(robot2Action->checkState() == Qt::Checked)
        checkBoxStates[2][0]=1;
    if(robot2Distance->checkState() == Qt::Checked)
        checkBoxStates[2][1]=1;

    if(robot3Action->checkState() == Qt::Checked)
        checkBoxStates[3][0]=1;
    if(robot3Distance->checkState() == Qt::Checked)
        checkBoxStates[3][1]=1;

    if(robot4Action->checkState() == Qt::Checked)
        checkBoxStates[4][0]=1;
    if(robot4Distance->checkState() == Qt::Checked)
        checkBoxStates[4][1]=1;

    if(robot5Action->checkState() == Qt::Checked)
        checkBoxStates[5][0]=1;
    if(robot5Distance->checkState() == Qt::Checked)
        checkBoxStates[5][1]=1;

    if(robot6Action->checkState() == Qt::Checked)
        checkBoxStates[6][0]=1;
    if(robot6Distance->checkState() == Qt::Checked)
        checkBoxStates[6][1]=1;
}

void MainWindow::getnumber(QString number)
{
    robotnumber = number.toInt();
}

void MainWindow::showButtons()
{
    if(robotnumber == 3)
    {
        robot4->hide();
        robot5->hide();
        robot6->hide();
        robot4Action->hide();
        robot5Action->hide();
        robot6Action->hide();
        robot4Distance->hide();
        robot5Distance->hide();
        robot6Distance->hide();
    }
    else if(robotnumber == 4)
    {
        robot4->hide();
        robot5->hide();
        robot6->hide();
        robot4Action->hide();
        robot5Action->hide();
        robot6Action->hide();
        robot4Distance->hide();
        robot5Distance->hide();
        robot6Distance->hide();
        robot4->show();
        robot4Action->show();
        robot4Distance->show();
    }
    else if(robotnumber == 5)
    {
        robot4->hide();
        robot5->hide();
        robot6->hide();
        robot4Action->hide();
        robot5Action->hide();
        robot6Action->hide();
        robot4Distance->hide();
        robot5Distance->hide();
        robot6Distance->hide();
        robot4->show();
        robot4Action->show();
        robot4Distance->show();
        robot5->show();
        robot5Action->show();
        robot5Distance->show();
    }
    else if(robotnumber == 6)
    {
        robot4->hide();
        robot5->hide();
        robot6->hide();
        robot4Action->hide();
        robot5Action->hide();
        robot6Action->hide();
        robot4Distance->hide();
        robot5Distance->hide();
        robot6Distance->hide();
        robot4->show();
        robot4Action->show();
        robot4Distance->show();
        robot5->show();
        robot5Action->show();
        robot5Distance->show();
        robot6->show();
        robot6Action->show();
        robot6Distance->show();
    }
    return;
}

void MainWindow::startCommand()
{
    system("roslaunch wolf new.launch");
}

void MainWindow::createStartThread()
{
    newThread = new std::thread(&MainWindow::startCommand,this);
    newThread->detach();
}

void MainWindow::endCommand()
{
    system("rosnode kill -a");
}

void MainWindow::createEndThread()
{
    newThread = new std::thread(&MainWindow::endCommand,this);
    newThread->detach();
}

void MainWindow::openWorld()
{
    QProcess::execute("gedit /home/zhang/catkin_ws/src/wolf/world/new.world");
}

void MainWindow::openLaunch()
{
    QProcess::execute("gedit /home/zhang/catkin_ws/src/wolf/launch/new.launch");
}
