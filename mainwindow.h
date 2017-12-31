#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QGroupBox;
class QLabel;
class QLineEdit;
class QPushButton;
class QRadioButton;
class QTableWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    //定义 作业 控制块 JCB
    typedef struct JCB{
          int id;           //ID
          QString name;     //作业名
          int arrival_time; //到达时间
          int start_time;   //开始时间
          int need_time;    //服务时间
          int runned_time;  //已经运行的时间
          int end_time;     //完成时间
          int priority;     //优先级
          QString status;   //状态
    }JCB;

    QVector<JCB> job_input, job_ready, job_run, result;     //QVector容器（数组）
    void Pri_Stablesort(QVector<JCB> *vector); //对 QVector<JCB> 按优先级 进行降序排序
    void Arr_Stablesort(QVector<JCB> *vector); //对 QVector<JCB> 按到达时间进行升序排序
    void Nee_Stablesort(QVector<JCB> *vector); //对 QVector<JCB> 按服务时间进行升序排序

    void J_setRunningTable(); //job[0]显示runningTable数据
    void J_setInputTable();   //job_input 数据显示到 readyTable
    void J_setReadyTable();   //job_ready 数据显示到 readyTable
    void J_setResultTable();  //result 数据显示到 resultTable
    void J_getReadyTable();   //获取 readyTable 数据到job_input


    void createJobPage();   //定义创建作业调度页面函数

    QWidget *jobPage;   //1

    QGroupBox *J_arithmeticGroupBox;//调度算法GroupBox
    QPushButton *FCFSbutton;        //静态优先权算法按钮
    QPushButton *SJFbutton;         //动态优先权算法按钮
    QPushButton *HRRNbutton;        //简单轮转法算法按钮
    QPushButton *J_Initbutton;      //初始化按钮

    QGroupBox *J_runningGroupBox;   //正在运行的进程GroupBox
    QTableWidget *J_runningTable;   //正在运行的进程表

    QGroupBox *J_readyGroupBox;     //就绪队列GroupBox
    QTableWidget *J_readyTable;     //就绪队列表

    QGroupBox *J_timeGroupBox;  //时间GroupBox
    QLabel *J_timeLabel;        //当前时间
    QLabel *J_TiLabel;          //服务时间标签
    QLabel *J_WiLabel;          //所需资源标签

    QGroupBox *J_resultGroupBox;    //调度结果GroupBox
    QTableWidget *J_resultTable;    //调度结果表

private slots:      //槽
    void FCFSclick();
    void FCFSone();
    void SJFclick();
    void SJFone();
    void HRRNclick();
    void HRRNone();
    void J_Initclick();
};

#endif // MAINWINDOW_H
