#ifndef _client_h
#define _client_h
#include "../typedefine.h"

ClientSocket_t *Client_open(char *ip, unsigned short port);
int Client_send(ClientSocket_t *des, dstream_t buff, len_t lengh);
int Client_recv(ClientSocket_t *des, dstream_t buff, len_t maxLen);
int Client_recv_wait(ClientSocket_t *des, dstream_t buff, len_t maxLen);
void Client_close(ClientSocket_t *local);

#endif // _client_h