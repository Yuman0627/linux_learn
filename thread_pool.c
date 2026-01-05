#include <glib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


//任务函数
void task_func(gpointer data, gpointer user_data)
{
    int task_num=*(int*)data;
    free(data);
    printf("任务%d开始执行\n",task_num);
    sleep(task_num);
    printf("任务%d执行完毕\n",task_num);
}


int main(int argc, char **argv)
{
    //创建线程池
    GThreadPool *pool=g_thread_pool_new(task_func,NULL,5,TRUE,NULL);

    //向线程池中添加任务
    for(size_t i=0;i<10;i++)
    {
        //用指针来表示每一个任务的编号
        int *tmp=malloc(sizeof(int)); 
        *tmp=i+1;
        g_thread_pool_push(pool,temp,NULL);    
    }
    g_thread_pool_free(pool,FALSE,TRUE);
    printf("所有的任务都完成了\n");

    return 0;
}