#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <string.h>
#include <semaphore.h>

int main(int argc, char *argv[])
{ 
    
    //创建信号量->使用共享内存创建
    char *shm_value_name="unname_sem_value";
    char *sem_name="/named_sem_shm";


    //创建内存共享对象
    int value_fd=shm_open(shm_value_name,O_RDWR|O_CREAT,0644);
    
    //初始化有名的信号量
    sem_t *sem=sem_open(sem_name,O_CREAT,0666,1);
    
    //配置共享内存对象的大小
    ftruncate(value_fd,sizeof(int));

    //映射共享内存对象
    char * value=mmap(NULL,sizeof(int),PORT_READ|PORT_WRITE,MAP_SHARED,value_fd,0);
    
    //初始化共享变量的值
    *value=0;
    //初始化信号量
    sem_init(sem,1,1);


    // 使用内存映射实现进程间的通讯
    pid_t pid=fork();
    if (-1==pid)
    {
        perror("fork");
    }else if (0==pid)
    {
        //子进程
        //信号量等待
        sem_wait(sem);
        int tmp = *value + 1;
        sleep(1);
        *value = tmp;
        //信号量唤醒
        sem_post(sem);

    }else{
        //父进程
        //信号量等待
        sem_wait(sem);
        int tmp = *value + 1;
        sleep(1);
        *value = tmp;
        //信号量唤醒
        sem_post(sem);

    }

    

    if (munmap(value, sizeof(int)) == -1)
    {
        perror("munmap value");
    }

    if (sem_close(sem) == -1)
    {
        perror("close sem");
    }

    // 无论父子进程都应该解除共享内存的映射，并关闭共享对象的文件描述符
    if (munmap(value, sizeof(int)) == -1)
    {
        perror("munmap value");
    }
    if (sem_close(sem) == -1)
    {
        perror("close sem");
    }

    // 如果调用时别的进程仍在使用共享对象，则等待所有进程释放资源后，才会销毁相关资源。
    // shm_unlink只能调用一次，这里在父进程中调用shm_unlink
    if(pid >0)
    {
        if (shm_unlink(shm_value_name) == -1)
        {
            perror("father shm_unlink shm_value_name");
        }
        if (shm_unlink(sem_name)==-1)
        {
            perror("father shm_unlink shm_sem_name");
        }
    }
    return 0;
    
}