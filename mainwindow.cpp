#include "mainwindow.h"
#include <QTimer>
#include "ui_mainwindow.h"
#include "cap.h"
#include "opencv_process.h"
#include "main.h"
#ifdef __cplusplus
    extern "C" {
#endif

#include "clientsocket.h"

#ifdef __cplusplus
    }
#endif

extern struct mainWorkStru 		gstMainWork;
extern struct capStru		capInfo;
extern FaceRecogn			gFaceRec;
extern FaceImageDeal        gFaceDeal;
extern struct socketInfo		clientSocketInfo;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    timer = new QTimer(this);       //刷新图像定时器
    timer1 = new QTimer(this);      //打卡定时器
    connect(timer, SIGNAL(timeout()), this, SLOT(showImage()));
    connect(timer1, SIGNAL(timeout()), this, SLOT(send_d()));
    timer->start(1);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showImage()
{
    if(!gstMainWork.workStatus)
    {
        timer->stop();
        return ;
    }
    timer->stop();
    gFaceDeal.detectFlag = 0;
    QImage img = gFaceDeal.cvMat2QImage(gFaceDeal.capMat);
    ui->label->setPixmap(QPixmap::fromImage(img));
    timer->start(1);
}

void MainWindow::on_pushButton_clicked()
{
    gstMainWork.cnt1 = 0;               //label清空
    gstMainWork.cnt2 = 0;
    ui->textMsg->setText("");           //textMsg清空
    timer1->start(5000);                //打卡定时器
}

void MainWindow::send_d()
{
    timer1->stop();                     //关闭打卡定时器
    if(gstMainWork.cnt1 > gstMainWork.cnt2) //进行5s的人脸检测和识别，发送所识别到人脸较多标签
    {
        
        if( send(clientSocketInfo.consockfd, "AB1", 3, 0) < 0)
        {
                printf("send msg error!\n");
                ui->textMsg->setText("打卡失败！！！！");
        }
        else
        {
            ui->textMsg->setText("杨超华打卡成功！");
        }
    }
    else if(gstMainWork.cnt1 < gstMainWork.cnt2)
    {
        if( send(clientSocketInfo.consockfd, "AB2", 3, 0) < 0)
        {
            printf("send msg error!\n");
            ui->textMsg->setText("打卡失败！！！！");
        }
        else
        {
            ui->textMsg->setText("王云路打卡成功！");
        }
    }
    else
    {
        ui->textMsg->setText("打卡失败！！！！");
    }
    
    gstMainWork.cnt1 = 0;
    gstMainWork.cnt2 = 0;
}
