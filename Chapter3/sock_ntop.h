#ifndef __SOCK_NTOP_H_
#define __SOCK_NTOP_H_
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
/*
    协议无关:socket address to string
*/
char* sock_ntop(const struct sockaddr* sa, socklen_t salen);
#endif//__SOCK_NTOP_H_