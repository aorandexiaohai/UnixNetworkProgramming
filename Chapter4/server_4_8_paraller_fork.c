#include "base_net.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in servaddr, cliaddr;
    bzero(&servaddr, sizeof(struct sockaddr_in));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(9999);
    if (bind(listenfd, (struct sockaddr *)&servaddr, sizeof(struct sockaddr)) != 0)
    {
        printf("绑定错误\n");
        exit(-1);
    }
    if (listen(listenfd, LISTENQ) != 0)
    {
        printf("监听错误\n");
        exit(-1);
    }
    pid_t pid;
    while (1)
    {
        printf("等待客户链接\n");
        char buff[MAXLINE] = {};
        socklen_t len = sizeof(struct sockaddr_in);
        int connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &len);
        
        if ((pid == fork()) == 0)
        {
            close(listenfd);
            //child process
            printf("connection from %s, port %d\n", inet_ntop(AF_INET, &cliaddr.sin_addr, buff, sizeof(buff)), ntohs(cliaddr.sin_port));
            time_t ticks = time(NULL);
            snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
            printf("开始传输\n");
            write(connfd, buff, strlen(buff));
            printf("结束传输\n");
            close(connfd);
            exit(0);
        }
        close(connfd);
    }
}