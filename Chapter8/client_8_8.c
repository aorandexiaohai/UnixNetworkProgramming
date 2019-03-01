#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include "base_net.h"
#include "sock_ntop.h"

void dg_cli(FILE *fp, int sockfd, const struct sockaddr_in *pservaddr, socklen_t servlen)
{
    char sendline[MAXLINE] = {};
    char recvline[MAXLINE + 1] = {};
    struct sockaddr *preply_addr = (struct sockaddr*)malloc(servlen);
    while (fgets(sendline, MAXLINE, fp) != NULL)
    {
        sendto(sockfd, sendline, strlen(sendline), 0, (const struct sockaddr *)pservaddr, servlen);
        socklen_t len = servlen;
        int n = recvfrom(sockfd, recvline, MAXLINE, 0, preply_addr, &len);
        //比较从服务器返回的地址是不是和发送目标地址一致
        /*
            这样比较仍然会有问题：
                当服务器是一个多宿主机时，服务器的外出接口可能不是客户端发送数据报的目的IP地址
                这样检测就会失败
        */
        if(len!=servlen || !memcmp(pservaddr, preply_addr, len))
        {
            printf("reply from %s (ignored)\n", sock_ntop((const struct sockaddr*)preply_addr, len));
            continue;
        }
        #if defined(DEBUG_ALL_PLATFORM)
        else
        {
            printf("reply from %s (accepted)\n", sock_ntop((const struct sockaddr*)preply_addr, len));
        }
        #endif//DEBUG_ALL_PLATFORM
        
        recvline[n] = 0;
        fputs(recvline, stdout);
    }
    free(preply_addr);
}

int main(int argc, char **argv)
{
    int port = atoi(getParameter2(argc, argv, "--serve_port", "9999"));
    const char *server_address = getParameter2(argc, argv, "--serve_address", "127.0.0.1");

    struct sockaddr_in servaddr;
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    inet_pton(AF_INET, server_address, &servaddr.sin_addr);

    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    dg_cli(stdin, sockfd, &servaddr, sizeof(servaddr));
}