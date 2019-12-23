#ifndef OPENCV_PROCESS_H
#define OPENCV_PROCESS_H
#include <opencv2/opencv.hpp>
#include <opencv2/face.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <QImage>
#include <QDebug>
#include <pthread.h>

#define MAT_SCALE		        3               //图像缩小比例，减少计算量
#define LBPH_REC_THRES_0		125.0	        //LBPH设定阈值/相似度0
#define LBPH_REC_THRES_80		85.0	        //LBPH识别过滤阈值/相似度80
#define LBPH_REC_THRES_100		65.0	        //LBPH相似度100阈值

using namespace cv;
using namespace std;
using namespace cv::face;

class FaceImageDeal
{
public:
    FaceImageDeal();
    int faceDeal_init(void);                    //人脸检测初始化 
    QImage cvMat2QImage(const Mat& mat);        //cv::Mat转为QImage格式
    void faceDeal_deInit(void);                 //人脸检测去初始化
                                                //人脸检测，并画框和现实姓名
    int detectAndDraw( Mat& img, CascadeClassifier& cascade, double scale);
public:
    Mat capMat;                                 //摄像头采集到的图片
    QImage QimgOpencv;                          //opencv转QImge格式图片
    int 				    detectFlag;			//检测到人脸标记 1-有
    Rect		            faceRect;           //人脸矩形
    Ptr<LBPHFaceRecognizer> modelLBPH;          //LBPH模型
    CascadeClassifier 	    face_cascade;       //人脸分类器

};

class FaceRecogn
{
public:
    FaceRecogn(void);
    int faceRec_init(void);                                 //人脸识别初始化
    int face_recognize(void);                               //人脸识别
    void faceRec_deInit(void);                              //人脸识别去初始化

public:
    Mat 						facesMat;					//检测提取的人脸
    int 						predFace;					//人脸编号 0-查无此人，>0-对应编号
    uchar						faceConfd;					//相似度/置信度
    int							predChang;					//更新结果

private:
    Ptr<LBPHFaceRecognizer> 	modelLBPH;			        //LBPH训练的人脸模型
    int 						predictLBPH;		        //LBPH识别结果


};


/* 图像处理/格式转化线程 */
void* faceopencv_handle(void *arg);

/* 人脸检测识别线程 */
void* faceDetRec_handle(void *arg);

/* opencv_process模块去初始化 */
void opencv_process_deInit(void);

/* 重启opencv两个线程 */
int opencv_process_restart(void);

#endif // OPENCV_PROCESS_H
