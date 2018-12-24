#include "base_net.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "Chapter3/read_write_helper.h"

void str_echo(int sockfd);

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