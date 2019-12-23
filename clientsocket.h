/*
客户端socket头文件
*/

#ifndef CLIENTSOCKET_H
#define CLIENTSOCKET_H
#include <stdio.h>
#include <string.h>
#include <asm/types.h>         
#include <fcntl.h>              
#include <unistd.h>
#include <errno.h>
#include <malloc.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <poll.h>
#include <pthread.h>

struct socketInfo
{
    int port;				                //端口
    int svrsockfd;		                    //无用的，server的
    int consockfd;		                    //用到的，用于连接的
    int connectSta;		                    //连接状态 0-disconnect , 1-connectted
    struct sockaddr_in 	serverAddr;		    //server 地址
    struct sockaddr_in 	clientAddr;		    //client 地址
    socklen_t 			serverlen;
    socklen_t 			clientlen;
};


void* socket_handle(void *arg);                             //客户端socket线程
int clientsocket_init(struct socketInfo *clientsocket);     //客户端socket初始化
void clientSocket_deInit(struct socketInfo *stSocket);      //客户端socket去初始化
#endif // CLIENTSOCKET_H
