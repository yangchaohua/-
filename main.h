#ifndef MAIN_H
#define MAIN_H
#include <stdio.h>
#include <string.h>
#include <pthread.h>





// 管理全局工作信息
struct mainWorkStru
{
    int			workStatus;		    //工作状态0-exit, 1-work
    pthread_t	tid_Sock;		    //socket 线程
    pthread_t   tid_cap;            //摄像头采集线程
    pthread_t	tid_facOpen;		// face opencv deal pthread
    pthread_t	tid_facDetRec;		//人脸识别检测线程
    int         label;              //标签
    int         cnt1;               //label 1计数器
    int         cnt2;               //label 2计数器
    int 		opcvDealReboot;		//opencv处理模块重启标志
};


#endif // MAIN_H
