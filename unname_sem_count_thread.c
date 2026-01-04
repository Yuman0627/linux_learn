#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

//声明两个信号量
sem_t *full;
sem_t *empty;

int shard_num;
int rand_num()
{
    strand(time(NULL));
    return rand();
}
void *producer(void *arg)
{ 
    for (size_t i = 0; i < 5; i++)
    {
        //获取信号量
        //最开始 empty为1， 可以减1 正常执行
        //下次到这里 empty为0， 无法减1， 阻塞在这里
        sem_wait(empty);
        printf("生产者%d\n",i+1);
        sleep(1);
        shared_num=read_num();
        //释放信号量
        //使full加1， 此时full为1  消费者就能获取full信号量
        sem_post(full);
    }

}

void *consumer(void *arg)
{ 
    for (size_t i = 0; i < 5; i++)
    {
        //获取信号量
        //full为0， 无法减1， 阻塞在这里
        sem_wait(full);
        printf("消费者%d\n",i+1);
        sleep(1);
        printf("接收到的数据是：%d\n",shared_num);
        //释放信号量
        //消费者执行一轮，释放信号使empty+1， 生产者就能获取信号量
        sem_post(empty);
    }
}

int main(int argc, char *argv[])
{
    //分配信号量
    full=malloc(sizeof(sem_t));
    empty=malloc(sizeof(sem_t));

    //初始化信号量
    sem_init(empty,0,1);
    sem_init(full,0,0);

    //创建生产者和消费者线程
    pthread_t producer_id,consumer_id;
    pthread_create(producer_id,NULL,producer,NULL);
    pthread_create(producer_id,NULL,consumer,NULL);

    //等待线程结束
    pthread_join(producer_id,NULL);
    pthread_join(consumer_id,NULL);

    //摧毁信号量
    sem_destroy(empty);
    sem_destroy(full);

    return 0; 
}
