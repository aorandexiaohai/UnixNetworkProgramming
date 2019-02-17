#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include "base_net.h"

void dg_echo(int sockfd, struct sockaddr_in* pcliaddr, socklen_t clilen)
{
    char mesg[MAXLINE] = {};
    while(1){
        socklen_t len = clilen;
        //flag参数为0，暂时不设置
        int n = recvfrom(sockfd, mesg, MAXLINE,0, (struct sockaddr*)pcliaddr,&len);
        sendto(sockfd, mesg, n,0, (struct sockaddr*)pcliaddr, len);
    }
    
}

int main(int argc, char** argv)
{
    int port = atoi(getParameter2(argc, argv, "--port", "9999"));

    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in servaddr, childaddr;
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(port);
    bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
    dg_echo(sockfd, &childaddr, sizeof(childaddr));
}