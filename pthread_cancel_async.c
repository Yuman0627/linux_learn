#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void * task(void *arg)
{
    printf("task start\n");
    //默认取消类型是延迟
    //设置取消类型为异步
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL);
    printf("working...");
    int i=0;
    while(1)
    {
        printf("%d\n",i++);
    }
    
    printf("task end\n");
    return NULL;
}

int main(int argc, char *argv[])
{

    //创建线程
    pthread_t tid;
    pthread_create(&tid,NULL,task,NULL);

    //取消子线程
    if (pthread_cancel(tid)!=0)
    {
        perror("pthread_cancel");
    }
    //pthread_cancel只是发出一个停止的命令
    void *res;
    pthread_join(tid,&res);
    if (res==PTHREAD_CANCELED)
    {
        printf("task canceled\n");

    }else{
        printf("task doesn't cancel. exit code %ld\n",(long)res);
    }


    //主线程运行完成比创建的子线程要慢，才能回收子线程
    printf("main thread continues\n");
    sleep(10);//因为主线程不会在pthread_detach等待 如果父进程先结束 会强制杀死子线程
    printf("main thread end\n");
    return 0;

}