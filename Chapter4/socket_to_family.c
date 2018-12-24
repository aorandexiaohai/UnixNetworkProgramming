#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

int socket_to_family(int fd)
{
    struct sockaddr_storage ss;
    socklen_t len = sizeof(ss);
    if(getsockname(fd, (struct sockaddr*)&ss, &len) < 0)
        return -1;
    return ss.ss_family;
}