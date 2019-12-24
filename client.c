#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#ifdef __MINGW32__
#include <winsock2.h>
#else
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#endif

#define MAXLINE 1024

int main(int argc, char **argv)
{
    int sockfd, n;
    char receline[MAXLINE + 1];
    struct sockaddr_in serveraddr;

    //输入参数太少，退出
    if (argc != 2)
    {
        printf("Usage :%s IP_address\n", argv[0]);
        exit(0);
    }

#ifdef __MINGW32__
    //Winsows下启用socket
    WSADATA wsadata;
    if (WSAStartup(MAKEWORD(1, 1), &wsadata) == SOCKET_ERROR)
    {
        printf("WSAStartup() fail\n");
        exit(0);
    }
#endif

    //建立socket
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        printf("socket() fail\n");
        exit(0);
    }

    //设置协议及Port
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(1024);

    //设置IP
    serveraddr.sin_addr.s_addr = inet_addr(argv[1]);

    //连接
    if (connect(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) == -1)
    {
        printf("connect() fail\n");
        exit(0);
    }

    //读取数据并输入到标准输出
    while ((n = recv(sockfd, receline, MAXLINE, 0)) > 0)
    {
        receline[n] = 0;
        if (fputs(receline, stdout) == EOF)
        {
            printf("fputs() error\r\n");
        }
    }

    //没有获取数据
    if (n < 0)
    {
        printf("read() fail\n");
    }

#ifdef __MINGW32__
    //Winsows下关闭socket
    closesocket(sockfd);
    WSACleanup();
#endif

    exit(0);
}