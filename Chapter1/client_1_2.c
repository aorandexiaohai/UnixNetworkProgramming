/*
    一个简单的时间获取客户程序
*/

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define MAXLINE 2048
int main(int argc, char** argv)
{
    int sockfd,n;
    char recvline[MAXLINE+1];
    struct sockaddr_in servaddr;
    if(argc != 2)
    {
        printf("参数错误\n");
        exit(-1);
    }
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("socket 初始化错误\n");
        exit(-1);
    }
    bzero(&servaddr, sizeof(struct sockaddr_in));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(9999);
    if(inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0)
    {
        printf("socket 错误\n");
        exit(-1);
    }
    if(connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0)
    {
        printf("链接错误\n");
        exit(-1);
    } 
    while((n=read(sockfd, recvline, MAXLINE)) > 0)
    {
        recvline[n] = 0;
        fputs(recvline, stdout);
    }
    if(n < 0)
    {
        printf("读取错误\n");
        exit(-1);
    }
    return 0;
}