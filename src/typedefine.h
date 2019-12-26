#ifndef _typedefine_h
#define _typedefine_h
#include "./cstd/cvector.h"
#include <stdio.h>

///< include of cross platform
#ifdef __MINGW32__
#include <winsock2.h>
#define sleep(n) Sleep(1000 * (n))
#else
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#endif //__MINGW32__

///< ------debug printf Port------------------------------
#if defined(DEBUG) || defined(_DEBUG)
#define dprintf(ftm, ...) printf(ftm, ##__VA_ARGS__)
#define K_PRINT dprintf
#define _DEBUG_LOG
#define _DEBUG_INFO
#define _DEBUG_WARNINR

#else
#define dprintf(ftm, ...)
#endif

#if defined(_DEBUG_LOG)
#define K_LOG(TYPE, fmt, ...)        \
    do                               \
    {                                \
        K_PRINT("  >[%s] :", #TYPE); \
        K_PRINT(fmt, ##__VA_ARGS__); \
    } while (0 == 1)
#else
#define K_LOG(TYPE, fmt, ...)
#endif // _DEBUG_LOG

#ifdef _DEBUG_INFO
#define K_INFOMATION(fmt, ...) K_LOG(Info, fmt, ##__VA_ARGS__)
#endif //_DEBUG_INFO

#ifdef _DEBUG_WARNINR
#define K_WARNINR(fmt, ...) K_LOG(Warning, fmt, ##__VA_ARGS__)
#endif //_DEBUG_WARNINR

#define K_ERROR(fmt, ...) K_LOG(Error, fmt, ##__VA_ARGS__)

///< -------------debug printf Port over------------------

typedef char *dstream_t;
typedef unsigned int len_t;

///< udp type define in there
typedef int UDP_FD;
typedef struct
{
    UDP_FD fd;
    unsigned short bind_port;
    char name[21];

} UDPSocket_t;

///< tcp client type define in there
typedef int CLIENT_FD;
typedef struct
{
    CLIENT_FD fd;
    unsigned short port;
    char name[21];
} ClientSocket_t;

///< tcp server type define in there
V_Define(ClientSocket_t *, ClientSocket_t);
typedef int SERVER_FD;
typedef struct
{
    SERVER_FD fd;
    unsigned short port;
    char name[21];
    VCT_ClientSocket_t *clients;
} ServerSocket_t;

///< OS thread define
#define OS_THREAD 1
#ifdef OS_THREAD
typedef void (*newSubThreadByClientSocket)(void *args); //传值等请根据自定义修改
#endif

///< mingw - socket prepare
#ifdef __MINGW32__
extern char initFlag;
#define Socket_prepare(defeatValue)                                     \
    do                                                                  \
    {                                                                   \
        if (initFlag == 0) /*if you see error was initFlag definition*/ \
        {                  /*you can define a char initFlag = 0;*/      \
            WSADATA wsadata;                                            \
            if (WSAStartup(MAKEWORD(1, 1), &wsadata) == SOCKET_ERROR)   \
            {                                                           \
                K_ERROR("WSAStartup() fail\n");                         \
                return defeatValue;                                     \
            }                                                           \
            initFlag++;                                                 \
        }                                                               \
    } while (0)

#else
#define Socket_prepare(defeatValue)
#endif

// 获取 域名和合法地址的IP  Get the IP of the domain name and the legal address
#define Socket_assert_IP(ipNum, ipChar, defeatValue)                       \
    do                                                                     \
    {                                                                      \
        struct hostent *host = gethostbyname(ipChar);                      \
        if (host == NULL)                                                  \
        {                                                                  \
            ipNum = inet_addr(ipChar);                                     \
            if (ipNum == INADDR_NONE)                                      \
            {                                                              \
                K_ERROR("Get IP address error!");                          \
                return defeatValue;                                        \
            }                                                              \
        }                                                                  \
        else                                                               \
        {                                                                  \
            ipNum = ((struct in_addr *)host->h_addr_list[0])->S_un.S_addr; \
        }                                                                  \
    } while (0)

#define Socket_VCT_contain(V, ele, defeatValue)                                        \
    do                                                                                 \
    {                                                                                  \
        u32 i = 0;                                                                     \
        for (; i < V->size; i++)                                                       \
        {                                                                              \
            if (ele->fd == V->data[i]->fd && strcmp(ele->name, V->data[i]->name) == 0) \
            {                                                                          \
                break;                                                                 \
            }                                                                          \
        }                                                                              \
        if (i == V->size)                                                              \
            return defeatValue;                                                        \
    } while (0)
#endif // _typedefine_h