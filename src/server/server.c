#include "server.h"
#include "../client/client.h"

#ifndef OS_THREAD
#error "this module must need OS Support"
#endif

V_Declare_Vector(ClientSocket_t *, ClientSocket_t);

ServerSocket_t *Server_open(unsigned short port, int maxConnet)
{
    struct sockaddr_in serveraddr;
    Socket_prepare(NULL);
    ServerSocket_t *server = (ServerSocket_t *)malloc(sizeof(ServerSocket_t));
    memset(server, 0, sizeof(ServerSocket_t));
    server->fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server->fd == -1)
    {
        K_ERROR("socket() fail\n");
        free(server);
        return NULL;
    }
    //清零
    memset(&serveraddr, 0, sizeof(serveraddr));

    //设置协议
    serveraddr.sin_family = AF_INET;
    //设置IP
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    //设置Port
    serveraddr.sin_port = htons(port);

    //绑定端口，监听1024端口的任何请求
    printf("bind()\n");
    if (bind(server->fd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) == -1)
    {
        K_ERROR("bind() fail\n");
        free(server);
        return NULL;
    }
    //监听端口，最大并发数10
    printf("listen()\n");
    if (-1 == listen(server->fd, maxConnet))
    {
        K_ERROR("listen() fail\n");
        free(server);
        return NULL;
    }
    server->clients = VCT_newVCT_ClientSocket_t();
    server->clients->deleteSub = (void (*)(const ClientSocket_t *))Client_close;
    return server;
}

ClientSocket_t *Server_hasNewConnection(ServerSocket_t *server)
{
#ifdef OS_THREAD
    return Server_waitNewConnection(server);
#else
    return NULL;
#endif
}

ClientSocket_t *Server_waitNewConnection(ServerSocket_t *server)
{
    CLIENT_FD clientfd = WSABASEERR;
    struct sockaddr saddr;
    int size = sizeof(saddr);
    K_INFOMATION("%s\n", "accept");
    clientfd = accept(server->fd, &saddr, &size);
    if (clientfd < WSABASEERR)
    {
        ClientSocket_t *subClient = (ClientSocket_t *)malloc(sizeof(ClientSocket_t));
        memset(subClient, 0, sizeof(ClientSocket_t));
        subClient->fd = clientfd;

        struct sockaddr_in *sock = (struct sockaddr_in *)&saddr;
        int port = ntohs(sock->sin_port);
#ifdef __MINGW32__
        sprintf(subClient->name, "%s:%d", inet_ntoa(sock->sin_addr), port);
#else
        struct in_addr in = sock->sin_addr;
        char str[INET_ADDRSTRLEN]; //INET_ADDRSTRLEN这个宏系统默认定义 16
        //成功的话此时IP地址保存在str字符串中。
        inet_ntop(AF_INET, &in, str, sizeof(str));
        sprintf(subClient->name, "%s:%d", str, port);
#endif
        if (server->clients != NULL)
            server->clients->append(server->clients, subClient);
        return subClient;
    }
    K_ERROR("%s\n", "accept");
    return NULL;
}

int Server_sendto(ServerSocket_t *local, ClientSocket_t *des, dstream_t buff, len_t lengh)
{
    if (local == NULL || des == NULL || buff == NULL || lengh == 0)
        return -2;
    Socket_VCT_contain(local->clients, des, -2);
    return Client_send(des, buff, lengh);
}

int Server_recvFrom(ServerSocket_t *local, ClientSocket_t *des, dstream_t buff, len_t maxlengh)
{
    return Server_recvFromWait(local, des, buff, maxlengh);
}

int Server_recvFromWait(ServerSocket_t *local, ClientSocket_t *des, dstream_t buff, len_t maxlengh)
{
    if (local == NULL || des == NULL || buff == NULL || maxlengh == 0)
        return -2;
    Socket_VCT_contain(local->clients, des, -2);
    return Client_recv(des, buff, maxlengh);
}

void Server_close(ServerSocket_t *local)
{
    if (local == NULL)
        return;
    if (local->clients != NULL)
        VCT_deleteVCT_ClientSocket_t(local->clients);
#ifdef __MINGW32__
    closesocket(local->fd);
#else
    close(local->fd);
#endif
    free(local);
}

#ifdef OS_THREAD
pthread_t Server_NewClinetThread(newSubThreadByClientSocket func, ServerSocket_t *server, ClientSocket_t *soc)
{
    pthread_t ret;
    K_INFOMATION("%s\n", "new thread");
    //void **agrs = (void **)malloc(sizeof(void *) * 2);
    void *agrs[2];
    agrs[0] = server;
    agrs[1] = soc;
    pthread_create(&ret, NULL, (void *)func, agrs);
    return ret;
}
#endif //OS_THREAD
