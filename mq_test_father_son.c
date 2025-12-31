#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

int main(void)
{

    //1.创建消息队列

    char *mq_name="/father_son_mq";
    struct mq_attr attr;
    //有用的属性，表示消息队列的容量
    attr.mq_maxsg=10;   //可以容纳的消息的最大数量
    attr.mq_msgsize=100;  //单条消息的大小，单位字节
    //被忽略的属性，创建消息队列时用不到
    attr.mq_flags=0;    //消息队列的属性
    attr.mq_curmsgs=0;  //当前消息队列中的消息数量

    mqd_t mqdes=mq_open(mq_name,O_RDWR|O_CREAT,0644,&attr);

    if(mqdes==(mqd_t)-1)
    {
        perror("mq_open");
        exit(EXIT_FAILURE);
    }

    //创建父子进程
    pid_t pid = fork();

    if (pid==-1)
    {
        perror(fork);
        exit(EXIT_FAILURE);
    }

    if (pid==0)
    {
        //子进程 接收mq中的消息，显示到控制台
        char read_buf[100];
        struct timespec time_info;
        for (size_t i=0;i<10;i++)
        {
            //清空buff
            memset(read_buf,0,sizeof(read_buf));

            //获取当前时间
            clock_gettime(CLOCK_REALTIME,time_info);
            //设置消息的过期时间，累加15秒
            time_info.tv_sec+=15;

            //接收消息,打印到控制台
            if (mq_timedreceive(mqdes,read_buf,100,NULL,&time_info)==-1)
            {
                perror("mq_receive");
            }
            printf("子进程接收消息：%s\n",read_buf);
        }

    }else{
        //父进程 发送消息到mq中
        char send_buf[100];
        struct timespec time_info;

        for (size_t i=0;i<10;i++)
        {
            //清空buff
            memset(send_buf,0,sizeof(send_buf));
            sprintf(send_buf,"父进程的第%d条消息\n",(int)(i+1));
            //获取当前时间
            clock_gettime(CLOCK_REALTIME,time_info);
            //设置消息的过期时间，累加5秒
            time_info.tv_sec+=5;
            //发送消息
            if (mq_timesend(mqdes,send_buf,strlen(send_buf),0,&time_info)==-1)
            {
                perror("mq_send");
            }
            printf("父进程发送消息，休眠一秒\n");
            sleep(1);
        }
    }

    //2.不管是父进程还是子进程，都需要释放消息队列的引用
    close(mqdes);

    //3.清除创建的消息队列(一方清就行)
    if (pid>0)
    {
        mq_unlink(mq_name);
    }

    return 0;
}