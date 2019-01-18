#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <poll.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>//for OPEN_MAX
#include <netinet/in.h>
#include <errno.h>
#include <sys/cdefs.h>
#include "base_net.h"
#include "Chapter3/read_write_helper.h"

#ifndef OPEN_MAX
#define OPEN_MAX 1024
#endif//OPEN_MAX

#ifndef INFTIM
#define INFTIM -1
#endif//INFTIM


int main(int argc, char** argv)
{
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in servaddr;
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(9999);
    bind(listenfd, (const struct sockaddr*)&servaddr, sizeof(servaddr));
    listen(listenfd, LISTENQ);

    struct pollfd client[OPEN_MAX];

    client[0].fd = listenfd;
    client[0].events = POLLRDNORM;
    for(int i=1; i<FOPEN_MAX; i++)
    {
        client[i].fd = -1;
    }
    int maxi = 0;
    while(1)
    {
        int nready = poll(client, maxi+1, INFTIM);
        if(client[0].revents & POLLRDNORM)
        {
            struct sockaddr_in cliaddr;
            socklen_t clilen = sizeof(cliaddr);
            int connfd = accept(listenfd, (struct sockaddr*)&cliaddr, &clilen);
            int i = 0;
            for(i=1; i<OPEN_MAX; i++)
            {
                if(client[i].fd < 0)
                {
                    client[i].fd = connfd;
                    break;
                }
            }
            if(i == OPEN_MAX)
            {
                err_msg("too many clients");
            }
            client[i].events = POLLRDNORM;
            if(i > maxi)
                maxi = i;
            if(--nready <= 0)
                continue;
        }
        for(int i=1; i<=maxi; i++)
        {
            int sockfd = client[i].fd;
            if( sockfd < 0 ) continue;
            int n = 0;
            char buf[MAXLINE] = {};
            if(client[i].revents & (POLLRDNORM | POLLERR))
            {
                /*
                    我们检查POLLERR的愿意在于:
                    有些实现在一个连接上接收到RST时返回POLLERR实现，有的则返回POLLERRNORM事件。
                    不论哪种情形，之后的read都会发生错误,返回-1
                */
                if((n=read(sockfd, buf, MAXLINE)) < 0)
                {
                    if(errno == ECONNRESET)//connection reset by client
                    {
                        close(sockfd);
                        client[i].fd = -1;
                    }
                    else
                    {
                        err_msg("read error\n");
                    }
                }
                else if(n == 0)//connection closed bt client
                {
                    close(sockfd);
                    client[i].fd = -1;
                }
                else
                    write(sockfd, buf, n);
                if(--nready <= 0) break;
            }
        }
    }
}