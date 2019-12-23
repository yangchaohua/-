/*
摄像头图像采集头文件
*/
#ifndef CAP_H
#define CAP_H
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <pthread.h>

using namespace cv;
struct capStru
{
    VideoCapture cam;                   //摄像头
    pthread_mutex_t frameLock;          //线程互斥锁
};

void* cap_handle(void *arg);            //摄像头采集线程
int cap_init(struct capStru *Info);     //摄像头初始化
int cap_getframe(struct capStru *Info); //获取1帧图像保存至Info.frame中
void cap_deInit(struct capStru *Info);  //摄像头去初始化
#endif // CAP_H
