#ifndef __BASE_NEXT_H_
#define __BASE_NEXT_H_
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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
//如果没有找到，则返回 0
static const char* getParameter(int argc, char** argv, const char* key)
{
    for(int i=1; i<argc-1; i++)
    {
        if(strcmp(argv[i], key) == 0)
        {
            printf("%s:%s\n",key,argv[i+1]);
            return argv[i+1];
        }
    }
    return 0;
}
//如果没有找到，则返回默认值
static const char* getParameter2(int argc, char** argv, const char* key, const char* default_value)
{
    const char* res = getParameter(argc, argv, key);
    if(!res)
    {
        printf("%s:%s\n",key,default_value);
        return default_value;
    }
    else return res;
}

#endif//__BASE_NEXT_H_