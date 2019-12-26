#ifndef _udp_h
#define _udp_h

#include "../typedefine.h"

UDPSocket_t *UDP_open(unsigned short port);
int UDP_send(UDPSocket_t *local, UDPSocket_t *des, dstream_t buff, len_t lengh);
int UDP_recv(UDPSocket_t *local, UDPSocket_t *des, dstream_t buff, len_t maxLen);
int UDP_recv_wait(UDPSocket_t *local, UDPSocket_t *des, dstream_t buff, len_t maxLen);
void UDP_close(UDPSocket_t *local);

#endif // _udp_h
