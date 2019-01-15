#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "base_net.h"
#include "Chapter3/read_write_helper.h"

/*
    拒绝服务型攻击
    如果一个恶心客户连接到该服务器后,发送一个字节的数据(不是换行符)后进入睡眠,将会发生什么?
    服务器将阻塞于read.

    服务器于是因为这么一个客户而被阻塞(称为挂起更确切些)，不能再为其他任何客户提供服务，直到那个客户发出一个换行符或者终止为止。

    上面的行为称为拒绝服务型攻击。
    可以解决的方法:
        1)使用非阻塞I/O
        2)为每个客户创建单独的子进程或者线程
        3)对I/O操作设置一个超时
*/


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

    int client[FD_SETSIZE] = {};

    for(int i=0; i<FD_SETSIZE; i++)
        client[i] = -1;

    fd_set allset;
    FD_ZERO(&allset);
    FD_SET(listenfd, &allset);

    int maxfd = listenfd;
    int maxi = -1;

    char buf[MAXLINE] = {};

    while(1)
    {
        fd_set rset = allset;
        int nready = select(maxfd+1, &rset, NULL, NULL, NULL);
        if(FD_ISSET(listenfd, &rset))
        {
            struct sockaddr_in cliaddr;
            socklen_t clilen = sizeof(cliaddr);
            //如果accept调用阻塞了,怎么办。
            int connfd = accept(listenfd, (struct sockaddr*)(&cliaddr), &clilen);
            int i = 0 ;
            for(i=0; i<FD_SETSIZE; i++)
            {
                if(client[i] < 0)
                {
                    client[i] = connfd;
                    break;
                }
            }
            if(i == FD_SETSIZE)
            {
                err_msg("too many clients!\b");
            }
            FD_SET(connfd, &allset);
            if(connfd > maxfd)
            {
                maxfd = connfd;
            }
            if( i > maxi)
                maxi = i;
            if(--nready <= 0)
                continue;//no more readable descriptors
        }
        for(int i=0; i<=maxi; i++)
        {
            int sockfd = client[i];
            if(sockfd < 0) continue;
            int n = 0;
            if(FD_ISSET(sockfd, &rset))
            {
                if((n==read(sockfd, buf, MAXLINE)) == 0)
                {
                    //connection closed by client.
                    close(sockfd);
                    FD_CLR(sockfd, &allset);
                    client[i] = -1;
                    //当客户关闭时,我们可以考虑减少maxi的值。
                }
                else 
                    writen(sockfd, buf, n);
                if(--nready <= 0)
                    break;//no more readable descriptors
            }
        }
    }
    
}