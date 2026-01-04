#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
int main(int argc, char const *argv[])
{
    char *shm_name = "unnamed_sem_shm";
    //创建共享内存
    int fd = shm_open(shm_name,O_CREAT | O_RDwR,0666);
    // 调整共享内存大小
    ftruncate(fd,sizeof(sem_t));

    // 映射共享内存
    sem_t *sem=mmap(NULL,sizeof(sem_t),PROT_READ | PROT_WRITE,MAP_SHARED,fd,0);
    
    // 初始化信号量
    sem_init(sem,1,0);//初始化设置为0

    //创建父子进程
    pid_t pid=fork();

    if(pid<0)
    {
        perror("fork");
    }

    if(pid==0)
    {
        //子进程
        sleep(1);
        printf("子进程执行\n");
        sem_post(sem);
        
    }
    else
    {
        //父进程
        sem_wait(sem);
        printf("父进程执行\n");
        waitpid(pid,NULL,0);
    }
    //回收资源
    if(pid>0)
    {
        if (sem_destroy(sem)==-1)
        {
            perror("sem_destroy");
    
        }
    }

    //父子进程都要关闭描述符
    if (mumap(sem,sizeof(sem))==-1)
    {
        perror("mumap");
    }
    if (close(fd)==-1)
    {
        perror("close");
    }

    
    if(pid>0)
    {
        if (shm_unlink(shm_name)==-1)
        {
            perror("shm_unlink");
        }
    }
    return 0;
}