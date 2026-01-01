#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUF_LEN 1024

char *buf;

// 读线程需要执行的代码逻辑
/* 先一个个读取一个字符
 * 缓冲区索引溢出时，索引重置为0
 */
void *input_thread(void *argv)
{
    int i=0;
    while(1)
    {
        char c=fgetc(stdin);
        if (c && c != '\n')// 判断是否有字符且是否为回车
        {
            buf[i++]=c; //先对buf[i]赋值c，再i++,循环继续执行
        }
        //缓冲区索引溢出
        if (i>=BUF_LEN)
        {
            i=0;
        }
    }
}

void *output_thread(void *argv)
{
    int i=0;
    while(1)
    {
        if(buf[i])
        {
            //读取一个字节写出到控制台 之后换行
            fputc(buf[i],stdout);
            fputc('\n',stdout);
            buf[i++]=0;  // 将缓冲区索引i对应的字符置为0，再i++，继续循环执行
            
            //缓冲区索引溢出，读到了最大下标
            if (i>=BUF_LEN)
            {
                i=0;
            }            
        }else{
            //如果当前还没有写入数据，休眠1秒
            sleep(1);
        }
    }
}


//程序实现创建两个线程
// 1. 读取控制台信息 写入到缓存中
// 2. 读取缓存信息 输出到控制台
int main(int argc, char *argv[])
{
    //初始化buf
    buf=malloc(BUF_LEN);

    //声明线程id
    pthread_t pid_input;
    pthread_t pid_output;

    //创建线程

    //创建读线程
    pthread_create(&pid_input,NULL,input_thread,NULL);

    //创建写线程
    pthread_create(&pid_output,NULL,output_thread,NULL);

    //主线程等待线程结束
    pthread_join(pid_input,NULL);
    pthread_join(pid_output,NULL);
    
    //释放buf
    free(buf);
    return 0;
}

