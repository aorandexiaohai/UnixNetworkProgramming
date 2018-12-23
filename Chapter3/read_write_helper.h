#ifndef __READ_WRITE_HELPER_H_
#define __READ_WRITE_HELPER_H_
#include <unistd.h>
int readn(int fd, void*vptr, int n);
int writen(int fd, const void* buff, int nbytes);
int readline(int fd, void*buff, int maxlen);

int readlineSpeedUpV1(int fd, void*buff, int maxlen);
#endif//__READ_WRITE_HELPER_H_