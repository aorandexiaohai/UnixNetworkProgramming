#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include "base_net.h"

void dg_cli(FILE* fp, int sockfd, const struct sockaddr_in* pservaddr, socklen_t servlen)
{
    char sendline[MAXLINE] = {};
    char recvline[MAXLINE + 1] = {};
    while(fgets(sendline, MAXLINE, fp) != NULL){
        sendto(sockfd, sendline, strlen(sendline),0,(const struct sockaddr*)pservaddr,servlen);
        /*
            第5/6个参数为空，这告知内核我们并不关心应答数据包由谁发送
            这样做存在一个风险:
                任何进程(本机和非本机)都可以向本客户的IP和端口发送数据报
        */
        int n=recvfrom(sockfd, recvline, MAXLINE,0,NULL,NULL);
        recvline[n] = 0;
        fputs(recvline, stdout);
    }
    
}


int main(int argc, char** argv)
{
    int port = atoi(getParameter2(argc, argv, "--serve_port", "9999"));
    const char* server_address = getParameter2(argc, argv, "--serve_address", "127.0.0.1");

    struct sockaddr_in servaddr;
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    inet_pton(AF_INET, server_address, &servaddr.sin_addr);

    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    dg_cli(stdin, sockfd, &servaddr, sizeof(servaddr));
}