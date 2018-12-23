#include "read_write_helper.h"
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "base_net.h"

int readn(int fd, void*vptr, int n)
{
    int nLeft    = n;
    int nread    = 0;
    char* ptr       = (char*)vptr;
    while(nLeft > 0)
    {
        if((nread=read(fd, ptr, nLeft)) < 0)
        {
            if(errno == EINTR)
                nread = 0; // call read() again
            else
                return -1;
        }
        else if(nread == 0)
        {
            /*EOF*/
            break;
        }
        nLeft -= nread;
        ptr += nread;
    }
    return n - nLeft;
}

int writen(int fd, const void* vptr, int n)
{
    int nLeft = n;
    const char* ptr = (const char*)vptr;
    int nwritten = 0;
    while(nLeft > 0)
    {
        if((nwritten=write(fd, ptr, nLeft)) <= 0)
        {
            if(nwritten<0 && errno==EINTR)
                nwritten = 0;//call write again
            else 
                return -1;//error
        }
        nLeft -= nwritten;
        ptr += nwritten;
    }
    return n;
}
int readline(int fd, void*buff, int maxlen)
{
    char* ptr = (char*)buff;
    int rc = 0;
    char c = 0;
    int n =0;
    for(n=1; n<maxlen; n++)
    {
        if((rc=read(fd, &c, 1)) == 1)
        {
            *ptr++ = c;
            if(c=='\n')
                break;
        }
        else if(rc == 0)
        {
            //EOF
            *ptr++ =0;
            return n-1;
        }
        else{
            if (errno == EINTR)
                continue;
            return -1;
        }
    }
    *ptr = 0;
    return n;
}





static int read_cnt = 0;
static char* read_ptr = NULL;
static char read_buff[MAXLINE];

static int my_read(int fd, char* ptr)
{
    if(read_cnt <= 0)
    {
        again:
        if((read_cnt=read(fd, read_buff, sizeof(read_buff))) < 0)
        {
            if(errno == EINTR)
                goto again;
            return -1;
        }
        else if(read_cnt == 0)//EOF
            return 0;
        read_ptr = read_buff;
    }
    read_cnt--;
    *ptr=*read_ptr++;
    return 1;
}

int readlineSpeedUpV1(int fd, void*buff, int maxlen)
{
    char* ptr = (char*)buff;
    int rc = 0;
    char c = 0;
    int n =0;
    for(n=1; n<maxlen; n++)
    {
        if((rc=my_read(fd, &c)) == 1)
        {
            *ptr++ = c;
            if(c=='\n')
                break;
        }
        else if(rc == 0)
        {
            //EOF
            *ptr++ =0;
            return n-1;
        }
        else{
            if (errno == EINTR)
                continue;
            return -1;
        }
    }
    *ptr = 0;
    return n;
}