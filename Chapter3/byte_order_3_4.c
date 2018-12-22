#include <stdio.h>
#include <assert.h>

int main(int argc, char** argv)
{
    union{
        short s;
        char c[sizeof(short)];
    } un;
    un.s = 0x0102;
    assert(sizeof(short) == 2);
    if(un.c[0]==1)
    {
        printf("big-endien\n");
    }
    else
    {
        printf("little-endien\n");
    }
}