/**
* @brief    作业调度实验
* @details  编写并调试一个单道处理系统的作业等待模拟程序。
*           作业等待算法：
*           分别采用先来先服务（FCFS）、
*           最短作业优先（SJF）、
*           响应比高者优先（HRN）的调度算法。
* @author   3115005124_张逸扬
* @date     2017年12月31日10:16:50
*/

#include "mainwindow.h"

#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QLayout>
#include <QGroupBox>
#include <QRadioButton>
#include <QTableWidget>
#include <QMessageBox>
#include <QTimer>
#include <QHeaderView>
//#include <QtAlgorithms>
#include <algorithm>

int Time;  //时钟 用于记录当前时间
QTimer *qtimer = new QTimer;    //时钟 用于控制显示速度快慢

//主页面
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle(tr("3115005124_张逸扬_操作系统实验_作业调度"));   //设置标题
    createJobPage();
    setCentralWidget(jobPage);
}

MainWindow::~MainWindow(){
}

//jobPage页面 布局
void MainWindow::createJobPage(){
    jobPage = new QWidget;

    //arithmeticGroupBox初始化及布局
    J_arithmeticGroupBox = new QGroupBox(tr("调度算法"));
    FCFSbutton = new QPushButton(tr("先来先服务算法"));
        connect(FCFSbutton, SIGNAL(clicked(bool)), this, SLOT(FCFSclick()));    //连接信号与槽
    SJFbutton = new QPushButton(tr("最短作业优先算法"));
        connect(SJFbutton, SIGNAL(clicked(bool)), this, SLOT(SJFclick()));
    HRRNbutton = new QPushButton(tr("响应比高者优先算法"));
        connect(HRRNbutton, SIGNAL(clicked(bool)), this, SLOT(HRRNclick()));
    J_Initbutton = new QPushButton(tr("初始化"));
        connect(J_Initbutton, SIGNAL(clicked(bool)), this, SLOT(J_Initclick()));

    //按钮初始化状态
    FCFSbutton->setEnabled(false);
    SJFbutton->setEnabled(false);
    HRRNbutton->setEnabled(false);

    QGridLayout *arithmeticLayout = new  QGridLayout;
    arithmeticLayout->setColumnStretch(0, 1);   //设置 QGridLayout 每一部分比例
    arithmeticLayout->setColumnStretch(1, 1);
    arithmeticLayout->setColumnStretch(2, 1);
    arithmeticLayout->setColumnStretch(3, 1);
    arithmeticLayout->setColumnStretch(4, 1);
    arithmeticLayout->addWidget(FCFSbutton, 0, 0);  //添加组件
    arithmeticLayout->addWidget(SJFbutton, 0, 1);
    arithmeticLayout->addWidget(HRRNbutton, 0, 2);
    arithmeticLayout->addWidget(J_Initbutton, 0, 4);
    J_arithmeticGroupBox->setLayout(arithmeticLayout);


    //P_runningGroupBox初始化及布局
    J_runningGroupBox = new QGroupBox(tr("正在运行的进程"));
    J_runningTable = new QTableWidget(1, 8);    //生成 1 行 8 列的表格

    //设置水平标题栏
    QStringList runningLabels;
    runningLabels << "ID" << "作业名" << "开始运行时间" << "已运行时间" << "还需运行时间" << "优先权" << "服务时间" << "状态";
    J_runningTable->setHorizontalHeaderLabels(runningLabels);

    J_runningTable->verticalHeader()->hide();   //隐藏垂直标题栏
    J_runningTable->setMinimumHeight(60);       //设置表格最小行高
    J_runningTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);     //设置自适应列宽
    J_runningTable->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);       //设置自适应行高
    J_runningTable->setEditTriggers(QAbstractItemView::NoEditTriggers);         //设置表格不可修改

    QGridLayout *runningLayout = new  QGridLayout;
    runningLayout->addWidget(J_runningTable);
    J_runningGroupBox->setLayout(runningLayout);


    //readyGroupBox初始化及布局
    J_readyGroupBox = new QGroupBox(tr("作业 输入/就绪 队列"));
    J_readyTable = new QTableWidget(5, 6);
    QStringList readyLabels;
    readyLabels << "ID" << "作业名称" << "提交时间" << "服务时间" << "优先权" << "状态";
    J_readyTable->setHorizontalHeaderLabels(readyLabels);

    J_readyTable->verticalHeader()->hide();   //隐藏垂直标题
    J_readyTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    J_readyTable->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    QGridLayout *readyLayout = new  QGridLayout;
    readyLayout->setColumnStretch(0, 2);
    readyLayout->setColumnStretch(1, 1);
    readyLayout->addWidget(J_readyTable, 0, 0);
    J_readyGroupBox->setLayout(readyLayout);


    //timeGroupBox初始化及布局
    J_timeGroupBox = new QGroupBox(tr("时间"));   //时间GroupBox
    J_timeLabel = new QLabel(tr("当前时刻：0"));
    J_TiLabel  = new QLabel(tr("平均周转时间：0"));
    J_WiLabel  = new QLabel(tr("平均带权周转时间：0"));

    QHBoxLayout *timeLayout = new QHBoxLayout;
    timeLayout->addStretch();
    timeLayout->addWidget(J_timeLabel);
    timeLayout->addStretch();
    timeLayout->addWidget(J_TiLabel);
    timeLayout->addStretch();
    timeLayout->addWidget(J_WiLabel);
    timeLayout->addStretch();
    J_timeGroupBox->setLayout(timeLayout);


    //P_resultGroupBox初始化及布局
    J_resultGroupBox = new QGroupBox(tr("调度结果"));
    J_resultTable = new QTableWidget(5, 9);
    QStringList resultLabels;
    resultLabels << "ID" << "作业名称" << "开始时间" << "服务时间" << "完成时间" << "优先权" << "状态" << "周转时间" << "带权周转时间";
    J_resultTable->setHorizontalHeaderLabels(resultLabels);

    J_resultTable->verticalHeader()->hide();
    J_resultTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    J_resultTable->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    J_resultTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    QGridLayout *resultLayout = new  QGridLayout;
    resultLayout->addWidget(J_resultTable);
    J_resultGroupBox->setLayout(resultLayout);


    //页面总布局
    QGridLayout *pageLayout = new QGridLayout;
    pageLayout->setMargin(0);
    pageLayout->setRowStretch(0,1);
    pageLayout->setRowStretch(1,1);
    pageLayout->setRowStretch(2,10);
    pageLayout->setRowStretch(3,1);
    pageLayout->setRowStretch(4,10);
    pageLayout->addWidget(J_arithmeticGroupBox, 0, 0);
    pageLayout->addWidget(J_runningGroupBox, 1, 0);
    pageLayout->addWidget(J_readyGroupBox, 2, 0);
    pageLayout->addWidget(J_timeGroupBox, 3, 0);
    pageLayout->addWidget(J_resultGroupBox, 4, 0);
    jobPage->setLayout(pageLayout);
}

//FCFS算法 按钮响应
void MainWindow::FCFSclick(){
    //运行期间 其他按钮 不可点击
    FCFSbutton->setEnabled(false);
    SJFbutton->setEnabled(false);
    HRRNbutton->setEnabled(false);
    J_Initbutton->setEnabled(false);

    J_getReadyTable();      //获取 readyTable 数据到 job

    qtimer->start(1000);     //设置时间间隔，200ms 执行 一次
    connect(qtimer, SIGNAL(timeout()), this, SLOT(FCFSone()));
}

//SJF算法 按钮响应
void MainWindow::SJFclick(){
    //运行期间 其他按钮 不可点击
    FCFSbutton->setEnabled(false);
    SJFbutton->setEnabled(false);
    HRRNbutton->setEnabled(false);
    J_Initbutton->setEnabled(false);

    J_getReadyTable();      //获取 readyTable 数据到 job

    qtimer->start(1000);     //设置时间间隔，200ms 执行 一次
    connect(qtimer, SIGNAL(timeout()), this, SLOT(SJFone()));
}

//HRRN算法 按钮响应
void MainWindow::HRRNclick(){
    //运行期间 其他按钮 不可点击
    FCFSbutton->setEnabled(false);
    SJFbutton->setEnabled(false);
    HRRNbutton->setEnabled(false);
    J_Initbutton->setEnabled(false);

    J_getReadyTable();      //获取 readyTable 数据到 job

    qtimer->start(1000);     //设置时间间隔，200ms 执行 一次
    connect(qtimer, SIGNAL(timeout()), this, SLOT(HRRNone()));
}

//FCFS算法 执行一次
void MainWindow::FCFSone(){
    //把 提交时间 <= 当前时间的作业 添加到 job_ready 容器中
    if(!job_input.isEmpty()){
        for(int i=0; i<job_input.size(); ){
            if(job_input[i].arrival_time<=Time){
                job_ready.append(job_input.takeAt(i));
                i=0;
            } else {
                i++;
            }
        }
    }

    Arr_Stablesort(&job_ready);     //job_ready 按服务时间排序
    //当 job_run 为空时，把 job_ready 首元素添加到 job_run 中
    if(job_run.isEmpty()){
        job_run.append(job_ready.first());
        job_ready.removeFirst();
    }
    job_run[0].status = "R";    //状态为Running
    job_run[0].runned_time++;
    //设开始时间
    if(job_run[0].start_time==-1 && job_run[0].runned_time==1){
        job_run[0].start_time=Time;
    }
    J_setRunningTable();
    //进程完成时
    if(job_run[0].runned_time == job_run[0].need_time){
        job_run[0].end_time=Time+1;
        job_run[0].status = "F";
        result.append(job_run[0]);
        job_run.removeFirst();
        J_setResultTable();
    }
    J_setReadyTable();
    //显示当前时间
    Time++;
    J_timeLabel->setText("当前时间：" + QString::number(Time));
    //当job为空 时
    if(result.size()==5){
        J_runningTable->clearContents();
        //计算 平均周转时间 Ti 和 平均带权周转时间 Wi
        double Ti_sum=0, Wi_sum=0;
        double Ti, Wi;
        for(int i=0; i<result.size(); i++){
            Ti_sum = Ti_sum + (double)(result[i].end_time - result[i].arrival_time);
            Wi_sum = Wi_sum + (double)((result[i].end_time - result[i].arrival_time) / (double)result[i].need_time);
        }
        Ti = Ti_sum / result.size();
        Wi = Wi_sum / result.size();
        J_TiLabel->setText("平均周转时间：" + QString::number(Ti));
        J_WiLabel->setText("平均带权周转时间：" + QString::number(Wi));
        qtimer->stop();
        qtimer->disconnect();
        QMessageBox::information(NULL, "完成", "作业调度（FCFS算法）完成");
        J_Initbutton->setEnabled(true);
    }
}

//SJF算法 执行一次
void MainWindow::SJFone(){
    //把 提交时间 <= 当前时间的作业 添加到 job_ready 容器中
    if(!job_input.isEmpty()){
        for(int i=0; i<job_input.size(); ){
            if(job_input[i].arrival_time<=Time){
                job_ready.append(job_input.takeAt(i));
                i=0;
            } else {
                i++;
            }
        }
    }

    Nee_Stablesort(&job_ready);     //job_ready 按服务时间排序
    //当 job_run 为空时，把 job_ready 首元素添加到 job_run 中
    if(job_run.isEmpty()){
        job_run.append(job_ready.first());
        job_ready.removeFirst();
    }
    job_run[0].status = "R";    //状态为Running
    job_run[0].runned_time++;
    //设开始时间
    if(job_run[0].start_time==-1 && job_run[0].runned_time==1){
        job_run[0].start_time=Time;
    }
    J_setRunningTable();
    //进程完成时
    if(job_run[0].runned_time == job_run[0].need_time){
        job_run[0].end_time=Time+1;
        job_run[0].status = "F";
        result.append(job_run[0]);
        job_run.removeFirst();
        J_setResultTable();
    }
    J_setReadyTable();
    //显示当前时间
    Time++;
    J_timeLabel->setText("当前时间：" + QString::number(Time));
    //当job为空 时
    if(result.size()==5){
        J_runningTable->clearContents();
        //计算 平均周转时间 Ti 和 平均带权周转时间 Wi
        double Ti_sum=0, Wi_sum=0;
        double Ti, Wi;
        for(int i=0; i<result.size(); i++){
            Ti_sum = Ti_sum + (double)(result[i].end_time - result[i].arrival_time);
            Wi_sum = Wi_sum + (double)((result[i].end_time - result[i].arrival_time) / (double)result[i].need_time);
        }
        Ti = Ti_sum / result.size();
        Wi = Wi_sum / result.size();
        J_TiLabel->setText("平均周转时间：" + QString::number(Ti));
        J_WiLabel->setText("平均带权周转时间：" + QString::number(Wi));
        qtimer->stop();
        qtimer->disconnect();
        QMessageBox::information(NULL, "完成", "作业调度（SJF算法）完成");
        J_Initbutton->setEnabled(true);
    }
}

//HRRN算法 执行一次
void MainWindow::HRRNone(){
    //把 提交时间 <= 当前时间的作业 添加到 job_ready 容器中
    if(!job_input.isEmpty()){
        for(int i=0; i<job_input.size(); ){
            if(job_input[i].arrival_time<=Time){
                job_ready.append(job_input.takeAt(i));
                i=0;
            } else {
                i++;
            }
        }
    }
    //计算 job_ready 里的优先权，R = （等待时间 + 要求服务时间） / 要求服务时间
    for(int i=0; i<job_ready.size(); i++){
        job_ready[i].priority = (Time - job_ready[i].arrival_time + job_ready[i].need_time) / job_ready[i].need_time;
    }
    Pri_Stablesort(&job_ready);     //job_ready 按优先级排序
    //当 job_run 为空时，把 job_ready 首元素添加到 job_run 中
    if(job_run.isEmpty()){
        job_run.append(job_ready.first());
        job_ready.removeFirst();
    }
    job_run[0].status = "R";    //状态为Running
    job_run[0].runned_time++;
    //设开始时间
    if(job_run[0].start_time==-1 && job_run[0].runned_time==1){
        job_run[0].start_time=Time;
    }
    J_setRunningTable();
    //进程完成时
    if(job_run[0].runned_time == job_run[0].need_time){
        job_run[0].end_time=Time+1;
        job_run[0].status = "F";
        result.append(job_run[0]);
        job_run.removeFirst();
        J_setResultTable();
    }
    J_setReadyTable();
    //显示当前时间
    Time++;
    J_timeLabel->setText("当前时间：" + QString::number(Time));
    //当job为空 时
    if(result.size()==5){
        J_runningTable->clearContents();
        //计算 平均周转时间 Ti 和 平均带权周转时间 Wi
        double Ti_sum=0, Wi_sum=0;
        double Ti, Wi;
        for(int i=0; i<result.size(); i++){
            Ti_sum = Ti_sum + (double)(result[i].end_time - result[i].arrival_time);
            Wi_sum = Wi_sum + (double)((result[i].end_time - result[i].arrival_time) / (double)result[i].need_time);
        }
        Ti = Ti_sum / result.size();
        Wi = Wi_sum / result.size();
        J_TiLabel->setText("平均周转时间：" + QString::number(Ti));
        J_WiLabel->setText("平均带权周转时间：" + QString::number(Wi));
        qtimer->stop();
        qtimer->disconnect();
        QMessageBox::information(NULL, "完成", "作业调度（HRRN算法）完成");
        J_Initbutton->setEnabled(true);
    }
}

//初始化按钮响应
void MainWindow::J_Initclick(){
    Time=0;
    J_timeLabel->setText("当前时间：0");
    J_TiLabel->setText("平均周转时间：0");
    J_WiLabel->setText("平均带权周转时间：0");
    job_input.clear();
    job_input.squeeze();
    result.clear();
    result.squeeze();
    J_resultTable->clearContents();

    //预输入数据
    job_input.append(JCB{1, "J1", 0, -1, 3, 0, -1, 3, "W"});
    job_input.append(JCB{2, "J2", 2, -1, 6, 0, -1, 5, "W"});
    job_input.append(JCB{3, "J3", 4, -1, 4, 0, -1, 1, "W"});
    job_input.append(JCB{4, "J4", 6, -1, 5, 0, -1, 2, "W"});
    job_input.append(JCB{5, "J5", 8, -1, 2, 0, -1, 4, "W"});
    J_setInputTable();

    FCFSbutton->setEnabled(true);
    SJFbutton->setEnabled(true);
    HRRNbutton->setEnabled(true);
}

//job[0]显示runningTable数据
void MainWindow::J_setRunningTable(){
    J_runningTable->clearContents();
    J_runningTable->setItem(0, 0, new QTableWidgetItem(QString::number(job_run[0].id)));  //ID
    J_runningTable->setItem(0, 1, new QTableWidgetItem(job_run[0].name));     //进程名
    J_runningTable->setItem(0, 2, new QTableWidgetItem(QString::number(job_run[0].start_time)));  //开始运行时间
    J_runningTable->setItem(0, 3, new QTableWidgetItem(QString::number(job_run[0].runned_time))); //已运行时间
    J_runningTable->setItem(0, 4, new QTableWidgetItem(QString::number(job_run[0].need_time - job_run[0].runned_time)));  //还需运行时间
    J_runningTable->setItem(0, 5, new QTableWidgetItem(QString::number(job_run[0].priority)));    //优先权
    J_runningTable->setItem(0, 6, new QTableWidgetItem(QString::number(job_run[0].need_time)));   //服务时间
    J_runningTable->setItem(0, 7, new QTableWidgetItem(job_run[0].status));   //状态
}

//job_input 数据显示到 readyTable
void MainWindow::J_setInputTable(){
    J_readyTable->clearContents();
    for(int i=0; i<job_input.size(); i++){
        J_readyTable->setItem(i, 0, new QTableWidgetItem(QString::number(job_input[i].id)));
        J_readyTable->setItem(i, 1, new QTableWidgetItem(job_input[i].name));
        J_readyTable->setItem(i, 2, new QTableWidgetItem(QString::number(job_input[i].arrival_time)));
        J_readyTable->setItem(i, 3, new QTableWidgetItem(QString::number(job_input[i].need_time)));
        J_readyTable->setItem(i, 4, new QTableWidgetItem(QString::number(job_input[i].priority)));
        J_readyTable->setItem(i, 5, new QTableWidgetItem(job_input[i].status));
    }
}

//job_ready 数据显示到 readyTable
void MainWindow::J_setReadyTable(){
    J_readyTable->clearContents();
    for(int i=0; i<job_ready.size(); i++){
        J_readyTable->setItem(i, 0, new QTableWidgetItem(QString::number(job_ready[i].id)));
        J_readyTable->setItem(i, 1, new QTableWidgetItem(job_ready[i].name));
        J_readyTable->setItem(i, 2, new QTableWidgetItem(QString::number(job_ready[i].arrival_time)));
        J_readyTable->setItem(i, 3, new QTableWidgetItem(QString::number(job_ready[i].need_time)));
        J_readyTable->setItem(i, 4, new QTableWidgetItem(QString::number(job_ready[i].priority)));
        J_readyTable->setItem(i, 5, new QTableWidgetItem(job_ready[i].status));
    }
}

//result 数据显示到 resultTable
void MainWindow::J_setResultTable(){
    J_resultTable->clearContents();
    for(int i=0; i<result.size(); i++){
        J_resultTable->setItem(i, 0, new QTableWidgetItem(QString::number(result[i].id)));            //ID
        J_resultTable->setItem(i, 1, new QTableWidgetItem(result[i].name));                           //进程名称
        J_resultTable->setItem(i, 2, new QTableWidgetItem(QString::number(result[i].start_time)));    //开始运行时间
        J_resultTable->setItem(i, 3, new QTableWidgetItem(QString::number(result[i].need_time)));     //服务时间
        J_resultTable->setItem(i, 4, new QTableWidgetItem(QString::number(result[i].end_time)));      //完成时间
        J_resultTable->setItem(i, 5, new QTableWidgetItem(QString::number(result[i].priority)));      //优先级
        J_resultTable->setItem(i, 6, new QTableWidgetItem(result[i].status));                         //状态
        J_resultTable->setItem(i, 7, new QTableWidgetItem(QString::number(result[i].end_time - result[i].arrival_time)));      //周转时间
        J_resultTable->setItem(i, 8, new QTableWidgetItem(QString::number((result[i].end_time - result[i].arrival_time) / (double)result[i].need_time)));    //带权周转时间
    }
}

//获取 readyTable 数据到job_input
void MainWindow::J_getReadyTable(){
    for(int i=0; i<job_input.size(); i++){
        job_input[i].id = J_readyTable->item(i, 0)->text().toInt();         //ID
        job_input[i].name = J_readyTable->item(i, 1)->text();               //进程名
        job_input[i].arrival_time = J_readyTable->item(i, 2)->text().toInt();   //到达时间
        job_input[i].need_time = J_readyTable->item(i, 3)->text().toInt();  //服务时间
        job_input[i].priority = J_readyTable->item(i, 4)->text().toInt();   //优先级
        job_input[i].status = J_readyTable->item(i, 5)->text();             //状态
    }
}

//对 QVector<JCB> 优先级 降序排序 使用Lambda表达式 std::stable_sort
void MainWindow::Pri_Stablesort(QVector<JCB> *vector){
    std::stable_sort(vector->begin(),vector->end(),[](const JCB &A,const JCB &B){
        return A.priority > B.priority;});
}

//对 QVector<JCB> 到达时间 升序排序 使用Lambda表达式 std::stable_sort
void MainWindow::Arr_Stablesort(QVector<JCB> *vector){
    std::stable_sort(vector->begin(),vector->end(),[](const JCB &A,const JCB &B){
        return A.arrival_time < B.arrival_time;});
}

//对 QVector<JCB> 服务时间 升序排序 使用Lambda表达式 std::stable_sort
void MainWindow::Nee_Stablesort(QVector<JCB> *vector){
    std::stable_sort(vector->begin(),vector->end(),[](const JCB &A,const JCB &B){
        return A.need_time < B.need_time;});
}
