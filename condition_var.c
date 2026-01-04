#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define BUFFER SIZE 5
int buffer[BUFFER_SIZE];
int count = 0;

//初始化互斥锁
static pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;
//初始化条件变量
static pthread_cond_t cond=PTHREAD_COND_INITIALIZER;

void * producer(void * arg)
{
    int item=1;

    //使用共同变量 使用互斥锁 首先获取锁
    //写满再读，锁放外面
    //pthread_mutex_lock(&mutex);
    while (1)
    {
        //使用共同变量 使用互斥锁 首先获取锁
        //写一个读一个
        pthread_mutex_lock(&mutex);

        //如果缓冲区写满 使用条件变量暂停当前线程
        if (count==BUFFER_SIZE)
        {
            //暂停线程
            pthread_cond_wait(&cond,&mutex);

        }
        //缓冲区没满
        buffer[count++]=item++;
        printf("Produce item %d\n",buffer[count-1]);

        //使用条件变量 唤醒消费者
        pthread_cond_signal(&cond);

        //最后释放锁
        pthread_mutex_unlock(&mutex);
    }

}
void * consumer(void * arg)
{
    while (1)
    {
        //使用共同变量 使用互斥锁 首先获取锁
        pthread_mutex_lock(&mutex);

        //如果缓冲区为空 使用条件变量暂停当前线程
        if (count==0)
        {
            //暂停线程
            pthread_cond_wait(&cond,&mutex);

        }
        //缓冲区不为空
        printf("Consume item %d\n",buffer[--count]);
        //使用条件变量 唤醒消费者
        pthread_cond_signal(&cond);

        //最后释放锁
        pthread_mutex_unlock(&mutex); 
    
    }
    
}

int main(int argc, char *argv[])
{
    //创建两个线程 一个线程负责生产者，一个线程负责消费者
    pthread_t producer_thread, consumer_thread;
    pthread_create(&producer_thread,NULL,producer,NULL);
    pthread_create(&consumer_thread,NULL,consumer,NULL);
    
    //主线程等待两个线程结束
    pthread_join(producer_thread,NULL);
    pthread_join(consumer_thread,NULL);
    
    
    return 0;
}