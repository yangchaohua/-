#include "clientsocket.h"
#include "main.h"

struct socketInfo		clientSocketInfo;
extern struct mainWorkStru 		gstMainWork;

/*
客户端socket线程
*/
void* socket_handle(void *arg)
{
    int	 	ret;
    ret = clientsocket_init(&clientSocketInfo);         //客户端socket初始化
    if(ret != 0)
    {
        printf("%s: clientsocket_init failed.\n", __FUNCTION__);
        return (void *)0;
    }
    printf("%s: clientsocket_init success.\n", __FUNCTION__);

    while(gstMainWork.workStatus)                       //当主程序处于工作状态时，进行连接
    {
        if(!clientSocketInfo.connectSta)	            //如果无连接，线程挂起1ms再次尝试连接
            usleep(1000);
    }

    clientSocket_deInit(&clientSocketInfo);             //当主程序关闭时，客户端socket去初始化
    return (void *)0;
}

/*
客户端socket初始化
*/
int clientsocket_init(struct socketInfo *clientsocket)
{
    int 	ret;
    clientsocket->connectSta = 0;
    clientsocket->consockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(clientsocket->consockfd < 0)
    {
        printf("socket() failed.\n");
        return -1;
    }
    printf("socket() success.\n");
    memset(&(clientsocket->serverAddr), 0, sizeof(clientsocket->serverAddr));
    clientsocket->serverAddr.sin_family = AF_INET;
    clientsocket->serverAddr.sin_addr.s_addr = inet_addr("192.168.43.160");
    clientsocket->serverAddr.sin_port = htons(5555);
    clientsocket->serverlen = sizeof(clientsocket->serverAddr);

    while(1)
    {
        printf("connecting......");
        ret = connect(clientsocket->consockfd, (struct sockaddr *)&clientsocket->serverAddr, clientsocket->serverlen);
        if(ret >= 0)
            break;
        usleep(20000);
    }
    clientsocket->connectSta = 1;	            //连接成功
    printf("connect(): success.\n");
    return 0;
}

void clientSocket_deInit(struct socketInfo *stSocket)
{
    printf("%s: enter.\n", __FUNCTION__);
    stSocket->connectSta = 0;
    if(stSocket->consockfd > 0)
    {
        close(stSocket->consockfd);
        stSocket->consockfd = -1;
    }

}

