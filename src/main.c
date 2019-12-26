#include "./client/client.h"
#include "./server/server.h"
#include "./udp/udp.h"
#include <stdio.h>
#include <time.h>

void subServerSocketHandel(void *args);

//#define SERVER
int main(void)
{
#ifdef CLIENT
    char buff[300];
    int n = 0;
    printf("hello world\n");
    ClientSocket_t *client = Client_open("127.0.0.1", 10001);
    while ((n = Client_recv(client, buff, 300)) > 0)
    {
        K_INFOMATION("\n%s\n", buff);
        K_INFOMATION("please input your data\n");
        scanf("%s", buff);
        if (Client_send(client, buff, strlen(buff)) <= 0)
            break;
    }
    Client_close(client);
    return 0;
#elif defined(SERVER)
    ServerSocket_t *server = Server_open(10001, 10);
    static int cnt = 0;
    while (cnt < 5)
    {
        Server_NewClinetThread(subServerSocketHandel, server, Server_hasNewConnection(server));
        cnt++;
    }
    return 0;
#elif defined(UDP)
    return 0;
#else
    printf("you shuld macro \"CLIENT\" or \"SERVER\" or \"\"UDP\"\n");
    return 0;
#endif
}
void subServerSocketHandel(void *args)
{

    ServerSocket_t *server = (ServerSocket_t *)((void **)args)[0];
    ClientSocket_t *client = (ClientSocket_t *)((void **)args)[1];
    char buff[300];
    //获取时间
    time_t tlick = time(NULL);
    //格式化时间
    snprintf(buff, sizeof(buff), "From mys:\n%s", ctime(&tlick));
    //写入时间
    //关闭请求
    K_INFOMATION("line :%d,clinet name = %s\n", __LINE__, client->name);
    send(client->fd, buff, strlen(buff), 0);
    K_INFOMATION("line :%d\n", __LINE__);
    int n = 0;
    while ((n = Client_recv(client, buff, sizeof(buff))) > 0)
    {
        if (strcmp(buff, "exit") == 0)
            break;
        buff[n] = 0;
        K_INFOMATION("recv from %s:\n%s\n", client->name, buff);
        Client_send(client, "OK", 2);
    }
    for (int i = 0; i < server->clients->getSize(server->clients); i++)
    {
        K_INFOMATION("line :%d\n", __LINE__);

        if (server->clients->at(server->clients, i)[0]->fd == client->fd &&
            strcmp(server->clients->at(server->clients, i)[0]->name, client->name) == 0)
        {
            K_INFOMATION("line :%d,i = %d,size = %d\n", __LINE__, i, server->clients->getSize(server->clients));
            server->clients->remove(server->clients, i, 1);
        }
    }
    //free(&server);
}