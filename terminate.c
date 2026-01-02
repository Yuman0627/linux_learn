#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <string.h>

//定义结构体接收线程结果
typedef struct Result{
    char *p;
    int len;
}Result;

//红玫瑰执行代码的函数
void * red_thread(void *arg)
{
    //初始化Result结构体
    Result *result=malloc(sizeof(Result));  
    //使用 malloc 在堆（heap）上动态分配一块内存空间，大小等于 Result 结构体的大小

    //解析传递的参数
    char code=*(char *)arg;
    //将 void * 类型的参数强制转换为 char * 类型，*(char *)arg：解引用转换后的指针，获取该地址处存储的字符值

    //*解地址，&取地址

    //声明存放读取消息的字符串
    char *ans=malloc(101);

    while (1)
    {
        fgets(ans,100,stdin);
        if (ans[0]==code)
        {
            //接收到了回复的信息
            free(ans);
            char *redans=strdup("红玫瑰独自去了纽约\n");
            /* 功能：复制字符串到新分配的内存中
                char *strdup("红玫瑰独自去了纽约\n");
                等效于：
                char *str = malloc(strlen("红玫瑰独自去了纽约\n") + 1);
                strcpy(str, "红玫瑰独自去了纽约\n");
            */
            result->p=redans;
            result->len=strlen(redans);
            //结束线程 返回故事结果
            pthread_exit((void *)result);
        }else{
            //没有接收到指定的消息
            printf("红玫瑰还在等你\n");
        }
    }
}

//白玫瑰执行代码的函数
void * white_thread(void *arg)
{
    //初始化Result结构体
    Result *result=malloc(sizeof(Result));

    //解析传递的参数
    char code=*(char *)arg;

    //声明存放读取消息的字符串
    char *ans=malloc(101);

    while (1)
    {
        fgets(ans,100,stdin);
        if (ans[0]==code)
        {
            //接收到了回复的信息
            free(ans);
            char *whiteans=strdup("白玫瑰独自去了巴黎\n");
            result->p=whiteans;
            result->len=strlen(whiteans);
            //结束线程 返回故事结果
            pthread_exit((void *)result);
        }else{
            //没有接收到指定的消息
            printf("白玫瑰还在等你\n");
        }
    }
}

int main(int argc, char *argv[])
{
    //海哥回复红玫瑰与白玫瑰为依据
    //创建两个线程
    pthread_t pid_red;
    pthread_t pid_white;
    char red_code='r'; //红玫瑰的code，指定线程接收到控制台发送的r后，红玫瑰才做出回复
    char white_code='w';

    Result *red_result=NULL; //定义用来存取结果返回值的指针
    Result *white_result=NULL;

    //红玫瑰线程
    pthread_create(&pid_red,NULL,red_thread,&red_code);
    
    //白玫瑰线程
    pthread_create(&pid_white,NULL,white_thread,&white_code);
    
    //获取红玫瑰线程结果
    pthread_join(pid_red,(void **)&red_result);
    printf("红玫瑰故事的结局%d是：%s\n",red_result,red_result->p);
    //释放内存
    free(red_result->p);
    free(red_result);

    //获取白玫瑰线程结果
    pthread_join(pid_white,(void **)&white_result);
    printf("白玫瑰故事的结局%d是：%s\n",white_result,white_result->p);
    free(white_result->p);
    free(white_result);

    return 0;
}

