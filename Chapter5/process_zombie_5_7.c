#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include<signal.h>

#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
int catch_dead_signal = 0;

void sig_chld(int signo)
{
    int stat = 0;
    pid_t pid = wait(&stat);
    fprintf(stdout, "process:%d is dead\n", pid);
    fflush(stdout);
    catch_dead_signal = 1;
}
int main()
{
    signal(SIGCHLD, sig_chld);
    pid_t pid = fork();

    if(pid == 0)
    {
        sleep(3);
        fprintf(stdout, "child process:%d\n", getpid());
        fflush(stdout);
        exit(0);
    }
    else
    {
        fprintf(stdout, "parent process:%d\n", getpid());
        fflush(stdout);
    }
    while(!catch_dead_signal)
        ;
}