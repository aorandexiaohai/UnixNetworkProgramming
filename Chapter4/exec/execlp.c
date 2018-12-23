#include <unistd.h>
#include <stdio.h>
int main()
{
    //第一个参数:如果带有\ 的话，则认为其是路径名
    //否则，在PATH环境变量下查找
    printf("result:%d", execlp("cat", "cat", "execl.c", NULL));
}