#include "opencv_process.h"
#include "main.h"

#ifdef __cplusplus
    extern "C" {
#endif

#include "clientsocket.h"

#ifdef __cplusplus
    }
#endif

using namespace std;
using namespace cv;

FaceImageDeal				gFaceDeal;
FaceRecogn					gFaceRec;
extern struct mainWorkStru 	gstMainWork;
extern struct socketInfo    clientSocketInfo;

/*
图像处理/格式转化线程
*/
void* faceopencv_handle(void *arg)
{
    printf("%s: enter.\n", __FUNCTION__);
    int ret = 0;
    int flag = 0;
    ret = gFaceDeal.faceDeal_init();                                                    //人脸检测初始化
    if(ret < 0)
    {
        printf("%s: faceDeal_init() failed.\n", __FUNCTION__);
        return (void *)0;
    }

    ret = pthread_create(&gstMainWork.tid_facDetRec, NULL, faceDetRec_handle, NULL);    //创建人脸检测识别线程
    if(ret < 0)
    {
        printf("pthread_create faceDetRec_handle failed.\n");
        return (void *)0;
    }

    while(gstMainWork.workStatus)
    {
        if(gstMainWork.opcvDealReboot)		                                            //重启线程
            break;
        if(!clientSocketInfo.connectSta)	                                            //无TCP连接
        {
            usleep(1000);
            continue;
        }
    }
    gFaceDeal.faceDeal_deInit();
    gstMainWork.opcvDealReboot++;
    printf("%s: exit ---------.\n", __FUNCTION__);
    return (void *)0;
}

/*人脸识别检测线程*/
void* faceDetRec_handle(void *arg)
{
    printf("%s: enter.\n", __FUNCTION__);
    int ret = 0;

    ret = gFaceRec.faceRec_init();                                  //人脸识别初始化
    if(ret < 0)
    {
        printf("%s: faceRec_init() failed.\n", __FUNCTION__);
        return (void *)0;
    }

    while(gstMainWork.workStatus)
    {
        if(gstMainWork.opcvDealReboot)		                        //重启线程
            break;

        if(gFaceRec.predChang)	                                    //已识别到人脸，未处理
        {
            usleep(1000);	
            continue;
        }

        if(gFaceDeal.capMat.empty())
        {
            usleep(10000);
            continue;
        }

        ret = gFaceDeal.detectAndDraw(gFaceDeal.capMat, gFaceDeal.face_cascade, MAT_SCALE); //人脸检测并框出人脸
        if(ret > 0)
        {
            ret = gFaceRec.face_recognize();                        //检测到人脸进行识别
        }
        else	// 无人脸
        {
            gFaceRec.faceConfd = 0;
        }

    }

    gFaceRec.faceRec_deInit();
    gstMainWork.opcvDealReboot ++;		                            //线程退出 信号 + 1
    printf("%s: exit ---------.\n", __FUNCTION__);

    return (void *)0;
}

/* 
重启opencv两个线程 
*/
int opencvDeal_restart(void)
{
    printf("\n------------------ %s: ------------------\n", __FUNCTION__);
    int ret = 0;
    gstMainWork.opcvDealReboot = 1;
    while(gstMainWork.opcvDealReboot <= 2)		                    //等待两个opencv处理线程退出
    {
        usleep(1000);
    }

    gstMainWork.opcvDealReboot = 0;

    ret = pthread_create(&gstMainWork.tid_facOpen, NULL, faceopencv_handle, NULL);  //创建图像处理/格式转化线程
    if(ret < 0)
    {
        printf("faceopencv_handle() failed.\n");
        return -1;
    }

    return 0;
}
FaceImageDeal::FaceImageDeal()
{
    printf("%s: enter.\n", __FUNCTION__);
}

/*
人脸处理初始化，使用opencv官方提供的人脸识别库
*/
int FaceImageDeal::faceDeal_init(void)
{
    int ret = 0;
    this->detectFlag = 0;

    ret = this->face_cascade.load("/usr/local/share/opencv4/haarcascades/haarcascade_frontalface_alt.xml");
    if( !ret )
    {
        printf("%s: load xml failed[ret=%d].\n", __FUNCTION__, ret);
        return -1;
    }

    return 0;
}

int FaceImageDeal::detectAndDraw( Mat& img, CascadeClassifier& cascade, double scale)
{
    vector<Rect> faces;
    Mat GrayImg_0;
    Mat GrayImg;
    cv::Point P1;
    int font_face = cv::FONT_HERSHEY_COMPLEX;
    double font_scale = 1;
    int thickness = 1;
    double t = 0;
    double fx = 1 / scale;
    cvtColor( img, GrayImg_0, COLOR_BGR2GRAY );                 //将源图像转为灰度图
    resize( GrayImg_0, GrayImg, Size(), fx, fx, INTER_LINEAR);	//缩放图像
    equalizeHist( GrayImg, GrayImg );	                        //直方图均衡化，提高图像质量
    t = (double)getTickCount();
    cascade.detectMultiScale( GrayImg, faces,
                                1.1, 2, 0
                                |CASCADE_SCALE_IMAGE,
                                Size(30, 30) );
    // 获取人脸矩形参数，只处理一张人脸，框出人脸并显示名字
    if(faces.size()>0)
    {
        gFaceDeal.faceRect.x = faces[0].x * scale;
        gFaceDeal.faceRect.y = faces[0].y * scale;
        gFaceDeal.faceRect.width = faces[0].width * scale;
        gFaceDeal.faceRect.height = faces[0].height * scale;
        gFaceDeal.detectFlag = 1;
        P1.x =  gFaceDeal.faceRect.x;
        P1.y =  gFaceDeal.faceRect.y;
        gFaceRec.facesMat = GrayImg(faces[0]);
        rectangle(gFaceDeal.capMat, gFaceDeal.faceRect, cv::Scalar(0, 255, 255), 2, 8, 0);
        if(gFaceRec.predFace  == 1)
            cv::putText(gFaceDeal.capMat,"YCH", P1, font_face, font_scale, cv::Scalar(0, 255, 255), thickness, 8, 0);
        else if(gFaceRec.predFace == 2)
            cv::putText(gFaceDeal.capMat,"WYL", P1, font_face, font_scale, cv::Scalar(0, 255, 255), thickness, 8, 0);
        return 1;
    }
    else
        return 0;
}

/*
将cv::Mat格式转为QImage格式
*/
QImage FaceImageDeal::cvMat2QImage(const Mat& mat)
{

    if(mat.empty())
        printf("Mat is empty.\n");
    QImage image((const uchar*)mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
    return image.rgbSwapped();

}

void FaceImageDeal::faceDeal_deInit(void)
{

}

/*
读取图片和标签，共两人，每人20张92x112的人脸图片
*/
static void read_txt(string filename, vector<Mat>& images, vector<int>& labels)
{
    int i = 0;
    cout << filename << endl;
    ifstream in(filename);
    if (!in) {
        printf( "No valid input file was given, please check the given filename.");
    }
    else
    {
        std::string  path;
        while (getline(in, path))
        {
            std::cout << path << endl;
            if(!path.empty())
            {
                images.push_back(imread(path, 0));
                if(i<20)                                //前20张为label 1
                    labels.push_back(1);
                else                                    //后20张为label 2
                    labels.push_back(2);
            }
            i++;
        }
    }
}

FaceRecogn::FaceRecogn()
{
    printf("%s: enter.\n", __FUNCTION__);
}

/*
人脸识别初始化
*/
int FaceRecogn::faceRec_init(void)
{
    this->predictLBPH = 0;
    this->predFace = 0;
    this->faceConfd = 0;
    this->predChang = 0;
    vector<Mat> images;
    vector<int> labels;
    std::string filepath = "/mnt/udisk/images/faces.txt";
    read_txt(filepath, images, labels);                 //读取图片和标签
    if(images.size() <= 1) {
        printf("This demo needs at least 2 images to work. Please add more images to your data set!");
    }

    //训练LBPH人脸识别模型
    this->modelLBPH = LBPHFaceRecognizer::create();
    this->modelLBPH->train(images, labels);     //训练
    modelLBPH->setThreshold(LBPH_REC_THRES_0);	// LBPH设置阈值
    printf("%s: --------- Face model train succeed ---------\n", __FUNCTION__);
    return 0;
}

/* 人脸识别，返回结果: 0-正常  -1-出错 */
int FaceRecogn::face_recognize()
{

    double confidence = 0.0;
    this->predictLBPH = 0;
    this->predFace = 0;

    Mat face_resize;
    if (this->facesMat.rows >= 60)  // 控制人脸不能太小(远)
    {
        resize(this->facesMat, face_resize, Size(92, 112));     //人脸resize至92x112
    }
    else
    {
        return -1;
    }

    if (!face_resize.empty())
    {
        modelLBPH->predict(face_resize, predictLBPH, confidence);
        if(predictLBPH > 0)
        {
            // 计算相似度
            if(confidence < LBPH_REC_THRES_100)
            {
                this->faceConfd = 100;
            }
            else if(confidence < LBPH_REC_THRES_80)
            {
                this->faceConfd = 80 + (LBPH_REC_THRES_80 - confidence) * 20 /
                                    (LBPH_REC_THRES_80 - LBPH_REC_THRES_100);
            }
            else if(confidence < LBPH_REC_THRES_0)
            {
                this->faceConfd = (LBPH_REC_THRES_0 - confidence) * 80 /
                                    (LBPH_REC_THRES_0 - LBPH_REC_THRES_80);
            }
            else
            {
                this->faceConfd = 0;
            }
        }
        else
        {
            this->faceConfd = 0;
        }
        /* 最终识别结果:相似度80以上 */
        if(this->predictLBPH > 0 && confidence < LBPH_REC_THRES_80)
        {
            this->predFace = predictLBPH;		        //label
            gstMainWork.label = this->predFace;
            cout << this->predFace <<endl;
            if(gstMainWork.label == 1)                  //label 1，计数器cnt1+1
                gstMainWork.cnt1++;
            if(gstMainWork.label == 2)                  //label 2，计数器cnt2+1
                gstMainWork.cnt2++;
        }
        else
        {
            this->predFace = 0;
            gstMainWork.label = 0;
        }
    }

    return 0;
}


void FaceRecogn::faceRec_deInit(void)
{

}

void opencv_process_deInit(void)
{
    gFaceDeal.faceDeal_deInit();
    gFaceRec.faceRec_deInit();
}
