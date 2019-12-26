#ifndef _server_h
#define _server_h

#include "../typedefine.h"

ServerSocket_t *Server_open(unsigned short port, int maxConnet);
ClientSocket_t *Server_hasNewConnection(ServerSocket_t *server);
ClientSocket_t *Server_waitNewConnection(ServerSocket_t *server);
int Server_sendto(ServerSocket_t *local, ClientSocket_t *des, dstream_t buff, len_t lengh);
int Server_recvFrom(ServerSocket_t *local, ClientSocket_t *des, dstream_t buff, len_t maxlengh);
int Server_recvFromWait(ServerSocket_t *local, ClientSocket_t *des, dstream_t buff, len_t maxlengh);
void Server_close(ServerSocket_t *local);

#ifdef OS_THREAD
#include <pthread.h>
pthread_t Server_NewClinetThread(newSubThreadByClientSocket func, ServerSocket_t *server, ClientSocket_t *soc);
#endif //OS_THREAD

#endif // _server_h