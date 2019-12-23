#include "mainwindow.h"
#include <QApplication>
#include <signal.h>
#include "main.h"
#include "opencv_process.h"
#include "cap.h"

#ifdef __cplusplus
    extern "C" {
#endif

#include "clientsocket.h"
#ifdef __cplusplus
    }
#endif

//全局信息
struct mainWorkStru 		gstMainWork;
extern struct socketInfo	clientSocketInfo;
struct capStru              capInfo;
extern FaceRecogn			gFaceRec;
extern FaceImageDeal        gFaceDeal;
void main_deInit(void);//release source

//Close program callback
void signal_exit_handler(int sig)
{
    printf("\n--------------- byebye ---------------\n");
    printf("%s: signal[%d].\n", __FUNCTION__, sig);
    main_deInit();
    exit(0);
}

int main(int argc, char *argv[])
{
    int ret;
    gstMainWork.workStatus = 1;
    gstMainWork.tid_Sock = 0;
    gstMainWork.tid_cap = 0;
    gstMainWork.tid_facOpen = 0;
    gstMainWork.tid_facDetRec = 0;
    gstMainWork.opcvDealReboot = 0;
    gstMainWork.label = 0;
    gstMainWork.cnt1 = 0;
    gstMainWork.cnt2 = 0;
    signal(SIGINT,  signal_exit_handler);
    signal(SIGABRT, signal_exit_handler);
    clientSocketInfo.port = 6666;

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    ret = pthread_create(&gstMainWork.tid_Sock, NULL, socket_handle, NULL);
    if(ret < 0)
    {
        printf("pthread_create socket_handle failed.\n");
        return -1;
    }

    ret = pthread_create(&gstMainWork.tid_facOpen, NULL, faceopencv_handle, NULL);
    if(ret < 0)
    {
        printf("faceopencv_handle faceopencv_handle failed.\n");
        return -1;
    }

    ret = pthread_create(&gstMainWork.tid_cap, NULL, cap_handle, NULL);
    if(ret < 0)
    {
        printf("cap_handle() failed.\n");
        return -1;
    }

    return a.exec();
}

void main_deInit(void)
{
    gstMainWork.workStatus = 0; 	//停止工作
    usleep(2000);		            //1 ms
    gFaceDeal.faceDeal_deInit();
    clientSocket_deInit(&clientSocketInfo);
}
