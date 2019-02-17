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
        sendto(sockfd, sendline, strlen(sendline),0,(const sockaddr*)pservaddr,servlen);
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