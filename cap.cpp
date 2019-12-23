#include "cap.h"
#include "opencv_process.h"
#include "main.h"

struct capStru capInfo;
extern struct mainWorkStru 		gstMainWork;
extern FaceImageDeal        gFaceDeal;

/*
摄像头采集线程
*/
void* cap_handle(void *arg)
{
    int ret = 0;
    ret = cap_init(&capInfo);                               //初始化摄像头capStru Info
    if(ret != 0)
    {
        printf("%s: cap_init failed.\n", __FUNCTION__);     //初始化失败
        return (void*)0;
    }

    while(gstMainWork.workStatus)                           //初始化成功，当主程序处于工作状态时，不停地获取图片
    {
        cap_getframe(&capInfo);
    }
    cap_deInit(&capInfo);
}

/*
初始化互斥锁，
摄像头初始化，利用cv::CAP_GSTREAMER打开摄像头，
s5p6818使用的CAM500B摄像头被friendlycore识别为/dev/video6，
图片格式为BGR方便opencv进行后续处理，帧率为30，图片尺寸为640x480，
成功打开摄像头返回0，失败返回-1。
*/
int cap_init(struct capStru *Info)
{
    pthread_mutex_init(&Info->frameLock, NULL);
    Info->cam = VideoCapture("v4l2src device=/dev/video6 ! video/x-raw,format=BGR,framerate=30/1,width=640,height=480 ! appsink",cv::CAP_GSTREAMER);
    if(!Info->cam.isOpened())
        return -1;
    printf("Camera open success!");
    return  0;
}

int cap_getframe(struct capStru *Info)
{
    pthread_mutex_lock(&Info->frameLock);                   //互斥锁打开
    Info->cam >> gFaceDeal.capMat;                          //读取一帧保存至cv::Mat gFaceDeal.capMat
    pthread_mutex_unlock(&Info->frameLock);                 //互斥锁关闭
}

/*
摄像头去初始化
*/
void cap_deInit(struct capStru *Info)
{
    pthread_mutex_destroy(&Info->frameLock);                //销毁互斥锁
    Info->cam.release();                                    //释放摄像头
}
