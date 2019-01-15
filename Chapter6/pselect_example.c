#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>

int sigint_event = 0;
void sigint_sig_handler(int s)
{ //设置信号处理函数
    sigint_event++;
    signal(SIGINT, sigint_sig_handler);
}
int main()
{
    int r;
    fd_set rd;
    sigset_t sigmask, orignal_sigmask; //设置新掩码和保存原始掩码
    FD_ZERO(&rd);                      //清空读描述符集合
    FD_SET(0, &rd);                    //将标准输入放入读描述符集合
    sigemptyset(&sigmask);             //清空信号
    sigaddset(&sigmask, SIGINT);       //将SIGINT信号加入sigmask中
    sigprocmask(SIG_BLOCK, &sigmask, &orignal_sigmask);
    signal(SIGINT, sigint_sig_handler); //挂接信号处理函数
    for (;;)
    {
        for (; sigint_event > 0; sigint_event--)
        {
            printf("sigint_event[%d]\n", sigint_event);
        }
        r = pselect(1, &rd, NULL, NULL, NULL, &orignal_sigmask); //pselect函数IO复用
        if (r == -1)
        {
            perror("pselect");
        }
        else if (r)
        {
            printf("Data is avaialble now\n");
        }
        else
        {
            printf("NO Data within five seconds\n");
        }
        sleep(1);
    }
    return 0;
}