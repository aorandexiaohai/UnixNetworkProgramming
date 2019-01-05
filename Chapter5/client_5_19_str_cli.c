#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

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
    char sendline[MAXLINE] = {};
    struct argsType as;
    struct resultType res;

    while(fgets(sendline, MAXLINE, fp) != NULL)
    {
        if(sscanf(sendline, "%d%d", &as.args1, &as.args2) != 2 )
        {
            printf("error input\n");
            continue;
        }
        printf("args:%d %d\n", as.args1,as.args2);
        if(writen(sockfd, &as, sizeof(as)) <= 0)
        {
            printf("error1\n");
            exit(-1);
        }
        if(readn(sockfd, &res, sizeof(res)) <= 0)
        {
            printf("error1\n");
            exit(-1);
        }
        printf("%d\n", res.sum);
    }
}