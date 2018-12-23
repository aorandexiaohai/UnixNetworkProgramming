#include <unistd.h>
#include <stdio.h>
int main()
{
    char* args[] = {
        "cat",
        "execl.c",
        NULL
    };
    printf("result:%d", execvp("cat", args));
}