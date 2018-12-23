#include <unistd.h>
#include <stdio.h>
int main()
{
    char *env_init[] = {"AA=aa","BB=bb",NULL};
    printf("result:%d", execle("/usr/bin/env", "env", NULL, env_init));
}