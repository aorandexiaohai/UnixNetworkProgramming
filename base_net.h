#ifndef __BASE_NEXT_H_
#define __BASE_NEXT_H_
#include <stdlib.h>

#define MAXLINE 1024
#define LISTENQ 5
struct argsType{
    int args1;
    int args2;
};
struct resultType{
    int sum;
};
static void err_msg(const char* str)
{
    printf("%s", str);
    fflush(stdout);
    exit(-1);
}
#endif//__BASE_NEXT_H_