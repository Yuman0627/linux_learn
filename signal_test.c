#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void signal_handler(int signum)
{
    printf("I'm handling a signal:%d\n",signum);
    exit(signum);
}

int main(int argc, char *argv[])
{
    //SIGINT:这是当用户在终端按下Ctrl+C时发送给前台进程的信号，通常用于请求进程终止。
    if (signal(SIGINT,signal_handler)==SIG_ERR)
    {
        perror("signal");
        return 1;
    }

    while (1)
    {
        sleep(1);
        printf("I'm alive\n");
    }

    return 0;
}