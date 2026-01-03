#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

pthread_rwlock_t rwlock;
int shared_data=0;

void * lock_writer(void *arg)
{
    //读写锁中的读是可以由多个线程同时访问的，但是读写锁中的写只能有一个线程进行访问
    pthread_rwlock_rdlock(&rwlock);
    printf("当前是%s,shared_data为%d\n",(char *)arg,shared_data);
    sleep(1);
    //释放读锁
    pthread_rwlock_unlock(&rwlock);
}
void * lock_writer(void *arg)
{
    //给多个线程写入添加写锁
    //同一个线程只能有一个写锁，会造成两个线程顺序执行
    pthread_rwlock_wrlock(&rwlock);
    int tmp=shared_data+1;
    shared_data=temp;
    printf("当前是%s,shared_data为%d\n",(char *)arg,shared_data);
    //写完后释放写锁
    pthread_rwlock_unlock(&rwlock);
}

int main(int argc, char *argv[])
{
    /*
        解决写饥饿问题
        写饥饿问题(Writer Starvation)是指在使用读写锁时，
        写线程可能无限期地等待获取写锁，
        因为读线程持续地获取读锁而不断地推迟写线程的执行。
        //创建读写锁属性对象
        pthread_rwlockattr_t_attr;
        pthread_rwlockattr_init(&attr);
        //修改参数 设置写优先
        pthread_rwlockattr_setkind_np(&attr,PTHREAD_RWLOCK_PREFER_WRITER_NONRECURSIVE_NP);
        //显示初始化读写锁
        pthread_rwlock_init(&rwlock,&attr);

    */

    //初始化锁
    pthread_rwlock_init(&rwlock,NULL);

    pthread_t writer1,writer2,read1,read2,raed3,read4;

    pthread_create(&writer1,NULL,lock_writer,"writer1");
    pthread_create(&writer2,NULL,lock_writer,"writer2");
    //休眠等待
    pthread_create(&read1,NULL,lock_reader,"read1");
    pthread_create(&read2,NULL,lock_reader,"read2");
    pthread_create(&read3,NULL,lock_reader,"read3");
    pthread_create(&read4,NULL,lock_reader,"read4");

    //主线程等待创建的子线程
    pthread_join(writer1,NULL);
    pthread_join(writer2,NULL);
    pthread_join(read1,NULL);
    pthread_join(read2,NULL);
    pthread_join(read3,NULL);
    pthread_join(read4,NULL);

    //销毁锁
    pthread_rwlock_destroy(&rwlock);

    return 0;
}
