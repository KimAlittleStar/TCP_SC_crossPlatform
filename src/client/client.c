#include "client.h"

#ifndef OS_THREAD
#error "this module must need OS Support"
#endif
char initFlag = 0;
ClientSocket_t *Client_open(char *ip, unsigned short port)
{
    unsigned int ipnum = 0;
    struct sockaddr_in serveraddr;
    Socket_assert_IP(ipnum, ip, NULL);
    Socket_prepare(NULL);
    ClientSocket_t *client = (ClientSocket_t *)malloc(sizeof(ClientSocket_t));
    memset(client, 0, sizeof(ClientSocket_t));

    //建立socket
    if ((client->fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        K_ERROR("socket() fail\n");
        free(client);
        return NULL;
    }

    //设置协议及Port
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(port);

    //设置IP
    serveraddr.sin_addr.s_addr = ipnum;

    //连接
    if (connect(client->fd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) == -1)
    {
        K_ERROR("connect() fail\n");
        free(client);
        return NULL;
    }
    sprintf(client->name, "%s:%d", inet_ntoa(serveraddr.sin_addr), port);
    return client;
}

/**
 ***********************************************************************
 * @brief Client_send
 * @param des                 你的目标 socket
 * @param buff                 数据buff
 * @param lengh                 buff 长度
 * @return int    成功则返回发送的长度,失败返回 -1 中途断开链接返回 0;
 * @author kimalittlestar@gmail.com
 * @date Create at 2019-12-25
 * @note AllCopyRight by XXX CO ,. LTD
 **********************************************************************
 */
int Client_send(ClientSocket_t *des, dstream_t buff, len_t lengh)
{
    if (des == NULL || buff == NULL || lengh == 0)
        return -1;
    return send(des->fd, buff, lengh, 0);
}

/**
 ***********************************************************************
 * @brief Client_recv
 * 等待至少一条数据接收完毕后,返回(若未收到数据,将阻塞)
 * @param des                 接收目标 socket
 * @param buff                 数据buff
 * @param lengh                 buff 允许的最大长度
 * @return int    成功则返回实际接收的数据的长度,失败返回 -1 中途断开链接返回 0;
 * @author kimalittlestar@gmail.com
 * @date Create at 2019-12-25
 * @note AllCopyRight by XXX CO ,. LTD
 **********************************************************************
 */
int Client_recv(ClientSocket_t *des, dstream_t buff, len_t maxLen)
{
    if (des == NULL || buff == NULL || maxLen == 0)
        return -1;
    return recv(des->fd, buff, maxLen, 0);
}

/**
 ***********************************************************************
 * @brief Client_recv_wait
 * 等待所有的数据接收/发送完毕(若未收到数据,将阻塞)
 * @param des                 接收目标 socket
 * @param buff                 数据buff
 * @param lengh                 buff 允许的最大长度
 * @return int    成功则返回实际接收的数据的长度,失败返回 -1 中途断开链接返回 0;
 * @author kimalittlestar@gmail.com
 * @date Create at 2019-12-25
 * @note AllCopyRight by XXX CO ,. LTD
 **********************************************************************
 */
int Client_recv_wait(ClientSocket_t *des, dstream_t buff, len_t maxLen)
{
    if (des == NULL || buff == NULL || maxLen == 0)
        return -1;
    return recv(des->fd, buff, maxLen, MSG_WAITALL);
}

void Client_close(ClientSocket_t *local)
{
    if (local == NULL)
        return;
#ifdef __MINGW32__
    closesocket(local->fd);
#else
    close(local->fd);
#endif
    free(local);
}
