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

    char write_buf[100];
    struct timespec time_info;

    while(1)
    {
        //清空缓冲区
        memset(write_buf,0,100);
        ssize_t read_count=read(STDIN_FILENO,write_buf,100);
        
        clock_gettime(REALTIME,&time_info);
        time_info.tv_sec+=5;

        //如果出错，则报错
        if (read_count==-1)
        {
            perror("read");
            continue;
        }else if (read_count==0)
        {
            //如果从控制台接收到停止发送的消息
            //ctrl+d 关闭控制台输入
            printf("EOF,exit...\n");
            char eof=EOF;
            //将EOF发送给消息队列
            if (mq_timesend(mqdes,eof,strlen(EOF),0,&time_info)==-1)
            {
                perror("mq_timesend");
            }
            break;
        }

        //正常接收到控制台的可读信息
        if (mq_timesend(mqdes,write_buf,strlen(write_buf),0,&time_info)==-1)
        {
            perror("mq_timesend");
        }
        printf("从控制台接收到消息，已经发送给消息队列\n");
    }

    //关闭消息队列描述符
    close(mqdes);

    return 0;
}
