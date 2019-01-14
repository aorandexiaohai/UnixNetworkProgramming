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
    int maxfdp1;
    fd_set rest;
    char sendline[MAXLINE]={}, recvline[MAXLINE]={};
    FD_ZERO(&rest);
    while(1)
    {
        int fp_fd = fileno(fp);
        FD_SET(fp_fd, &rest);
        FD_SET(sockfd, &rest);
        maxfdp1 = (fp_fd > sockfd ? fp_fd : sockfd) + 1;
        select(maxfdp1, &rest, NULL, NULL, NULL);
        if(FD_ISSET(sockfd, &rest))
        {
            if(readline(sockfd, recvline, MAXLINE) == 0)
            {
                err_msg("str_cli:server terminated prematurely.");
            }
            fputs(recvline, stdout);
        }
        if(FD_ISSET(fp_fd, &rest))
        {
            if(fgets(sendline, MAXLINE, fp) == NULL)
                return;//all done
            writen(sockfd, sendline, strlen(sendline));
        }
    }
}