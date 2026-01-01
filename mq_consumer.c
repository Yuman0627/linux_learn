#include <time.h>
#include <mqueue.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h> 
#include <string.h>
//生产者，往消息队列里发送消息
int main(int argc, char *argv[])
{
    //创建消息队列
    struct mq_attr attr;
    attr.mq_maxmsg=10;
    attr.mq_msgsize=100;

    attr.mq_flags=0;
    attr.mq_curmsgs=0;

    char *mq_name="/producer_consumer_queue";

    mqd_t mqdes=mq_open(mq_name,O_RDWR|O_CREAT,0644,&attr);

    if (mqdes==(mqd_t)-1)
    {
        perror("mq_open");
        exit(EXIT_FAILURE);
    }
    

    //不断发送消息（从控制台接收到的消息）

    char read_buf[100];
    struct timespec time_info;

    while(1)
    {
        //清空缓冲区
        memset(write_buf,0,100);
        //ssize_t read_count=read(STDIN_FILENO,read_buf,100);
        
        clock_gettime(REALTIME,&time_info);
        time_info.tv_sec+=5;

        //接收消息队列的一条信息
        if (mq_timereceive(mqdes,read_buf,strlen(read_buf),NULL,&time_info)==-1)
        {

            perror("mq_timereceive");
        }
        //判断当前数据是否为EOF，结束信息
        if (read_buf[0]==EOF)
        {
            printf("接收到生产者发送的结束信息，准备退出\n");
            break;
        }

        //正常读取的消息队列的信息打印到控制台
        printf("接收到来自生产者发送的信息%s\n",read_buf);
    }

    //关闭消息队列描述符
    close(mqdes);

    //清楚消息队列
    mq_unlink(mq_name);

    return 0;
}
