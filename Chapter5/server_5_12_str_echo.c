/*
网络编程遇到的三种情况:
1)当fork子进程时, 必须捕获SIGCHLD信号
2)当捕获信号时，必须处理被中断的系统调用
3)SIGCHLD的信号处理函数必须正确编写，应使用waitpid函数以免留下僵尸进程
*/
#include "base_net.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include<signal.h>

#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "Chapter3/read_write_helper.h"

void sig_chld(int signo)
{
    int stat = 0;
    pid_t pid = 0;
    while((pid = waitpid(-1, &stat, WNOHANG)) > 0)
    {
        printf("process %d terminated\n", pid);
    }
}

void str_echo(int sockfd);

int main()
{
    signal(SIGCHLD, sig_chld);

    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in servaddr, cliaddr;
    bzero(&servaddr, sizeof(struct sockaddr_in));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(9999);
    if (bind(listenfd, (struct sockaddr *)&servaddr, sizeof(struct sockaddr)) != 0)
    {
        exit(-1);
    }
    if (listen(listenfd, LISTENQ) != 0)
    {
        exit(-1);
    }
    pid_t pid;
    while (1)
    {
        char buff[MAXLINE] = {};
        socklen_t len = sizeof(struct sockaddr_in);
        int connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &len);
        if(connfd < 0)
        {
            if(errno == EINTR)
                continue;
            else
            {
                printf("accept error\n");
                exit(-1);
            }
        }

        if ((pid == fork()) == 0)
        {
            close(listenfd);
            time_t ticks = time(NULL);
            snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
            str_echo(connfd);
            close(connfd);
            exit(0);
        }
        close(connfd);
    }
}

void str_echo(int sockfd)
{
    int n;
    char buf[MAXLINE] = {};
again:
    while ((n = read(sockfd, buf, MAXLINE)) > 0)
    {
        if (writen(sockfd, buf, n) <= 0)
        {
            printf("error 1\n");
            exit(-1);
        }
    }
    if(n<0 && errno == EINTR)
    {
        goto again;
    }
    else if(n < 0)
    {
        printf("error 2\n");
        exit(-1);
    }
}