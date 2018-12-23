#include <unistd.h>
#include <stdio.h>
int main()
{
    printf("result:%d", execl("/bin/cat", "cat", "execl.c", NULL));
}