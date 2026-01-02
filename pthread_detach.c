#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void * task(void *arg)
{
    printf("task start\n");
    sleep(5);
    printf("task end\n");
    return NULL;
}

int main(int argc, char *argv[])
{

    //创建线程
    pthread_t tid;
    pthread_create(&tid,NULL,task,NULL);

    //使用detach标记会等待线程完成后回收相关资源
    pthread_detach(tid);

    //主线程运行完成比创建的子线程要慢，才能回收子线程
    printf("main thread continues\n");
    sleep(10);//因为主线程不会在pthread_detach等待 如果父进程先结束 会强制杀死子线程
    printf("main thread end\n");
    return 0;

}