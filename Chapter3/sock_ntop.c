#include "sock_ntop.h"
#include <stdio.h>
#include <string.h>

char *sock_ntop(const struct sockaddr *sa, socklen_t salen)
{
    char portstr[8] = {};
    static char str[128] = {};

    switch (sa->sa_family)
    {
    case AF_INET:
    {
        const struct sockaddr_in *sin = (const struct sockaddr_in *)(sa);
        if (inet_ntop(AF_INET, &sin->sin_addr, str, sizeof(str)) == 0)
            return 0;
        if (ntohs(sin->sin_port) != 0)
        {
            snprintf(portstr, sizeof(portstr), ":%d", ntohs(sin->sin_port));
            strcat(str, portstr);
        }
    }
    break;
    //to do for AF_INET6 and more...
    default:
        break;
    }
    return str;
}