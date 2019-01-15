#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "Chapter3/read_write_helper.h"
#include "base_net.h"
void str_cli(FILE* fp, int sockfd);

// #define MAXLINE 2048
int main(int argc, char** argv)
{
    int sockfd;
    char recvline[MAXLINE+1];
    struct sockaddr_in servaddr;
    if(argc != 2)
    {
        exit(-1);
    }
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        exit(-1);
    }
    bzero(&servaddr, sizeof(struct sockaddr_in));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(9999);
    if(inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0)
    {
        exit(-1);
    }
    if(connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0)
    {
        exit(-1);
    } 
    str_cli(stdin, sockfd);
    return 0;
}


void str_cli(FILE* fp, int sockfd)
{
    int maxfdp1 = 0;
    int stdineof = 0;
    fd_set rset;
    char buf[MAXLINE] = {};
    int n= 0;
    FD_ZERO(&rset);
    int fp_fd = fileno(fp);
    int out_fd = fileno(stdout);

    while(1)
    {
        if(stdineof == 0)
            FD_SET(fp_fd, &rset);
        FD_SET(sockfd, &rset);
        maxfdp1 = (fp_fd > sockfd ? fp_fd : sockfd) + 1;
        select(maxfdp1, &rset, NULL, NULL, NULL);
        if(FD_ISSET(sockfd, &rset))
        {
            if((n=read(sockfd, buf, MAXLINE)) == 0)
            {
                if(stdineof ==1)
                    return;//normal termination
                else
                    err_msg("str_cli: server terminated prematurely");
            }
            write(out_fd, buf, n);
        }
        if(FD_ISSET(fp_fd, &rset))
        {
            if((n=read(fp_fd, buf, MAXLINE)) == 0)
            {
                stdineof = 1;
                shutdown(sockfd, SHUT_WR);//send FIN
                FD_CLR(fp_fd, &rset);
                continue;
            }
            write(sockfd, buf, n);
        }
        

    }
}