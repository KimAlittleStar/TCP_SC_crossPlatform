#include "./client/client.h"
#include "./server/server.h"
#include "./udp/udp.h"
#include <stdio.h>

int main(void)
{
    char buff[300];
    int n = 0;
    printf("hello world\n");
    ClientSocket_t *client = Client_open("127.0.0.1", 10001);
    while ((n = Client_recv(client, buff, 300)) > 0)
    {
        K_INFOMATION("\n%s\n", buff);
    }
    Client_close(client);
    return 0;
}