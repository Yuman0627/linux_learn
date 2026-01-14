#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>


//宏定义只能写一行，/表示连接
#define handle_error(cmd, result) \
    if (result < 0)               \
    {                             \
        perror(cmd);              \
        return -l;                \
    }    
    
void *read_from_client(void *arg)
{
    //使用recv接收客户端发送的数据 打印到控制台
    char * read_buf=NULL;
    int clientfd=*(int *)arg;
    read_buf=malloc(sizeof(char) * 1024);
    ssize_t read_len=0;
    if (!read_buf)
    {
        perror("malloc");
        return -1;
    }
    //接收数据
    while(read_len=recv(clientfd,read_buf,1024,0))
    {
        fputs(read_buf,stdout);
        if (read_len<0)
        {
            perror("recv");
        }
    }

    printf("客户端已退出\n");
    free(read_buf);

    return NULL;

}

void *write_to_client(void *arg)
{
    //接收控制台的信息 写出去
    char * write_buf=NULL;
    int clientfd=*(int *)arg;
    write_buf=malloc(sizeof(char) * 1024);
    ssize_t write_len=0;
    if (!write_buf)
    {
        perror("malloc");
        return -1;
    }
    while(fgets(write_buf,1024,stdin)!=NULL)
    {
        //发送数据
        send(clientfd,write_buf,1024,0);
        if (write_len<0)
        {
            perror("send");
        }
    }
    printf("接收到控制台的关闭请求 不在写入 关闭连接\n");
    //可以具体到关闭某一端
    shutdown(clientfd,SHUT_WR);
    free(write_buf);
    return NULL;
}

int main(int argc, char *argv[])
{
    int sockfd,clientfd,temp_result;
    pthread_t pid_read,pid_write;

    struct socket_in server_addr,client_addr;
    //清空
    memset(&server_addr,0,sizeof(server_addr));
    memset(&client_addr,0,sizeof(client_addr));
    
    //填写服务端地址，不填，主要用于接收的
    server_addr.sin_family=AF_INET;
    //填写ip地址 0.0.0.0
    server_add.sin_addr.s_addr=htol(INADDR_ANY);
    //填写端口号
    server_addr.sin_port=htons(6666);

    //服务端网络编程流程
    //1.socket
    sockfd=socket(AF_INET,SOCK_STREAM,0);
    handle_error("socket",sockfd);

    //2.bind绑定地址
    temp_result=bind(sockfd,server_add,sizeof(server_addr));
    handle_error("bind",result);

    //3.listen监听
    temp_result=listen(sockfd,128);
    handle_error("listen",result);

    //4.accept接收获取客户端的连接
    socklen_t cliaddr_len=sizeof(client_addr);//获取地址长度
    
    //返回的文件描述符才是能和客户端收发信息的文件描述符
    //如果调用accept之后没有客户端连接 这里会挂起等待
    clientfd=accept(sockfd,&client_addr,&cliaddr_len);
    handle_error("accept",clientfd);

    printf("与客户端%s %d建立连接 文件描述符是%d\n",inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port),clientfd);


    //创建子线程用于收消息
    pthread_create(&pid_read,NULL,read_from_client,(void *)&clientfd);

    //创建子线程用于发消息
    pthread_create(&pid_write,NULL,write_to_client,(void *)&clientfd);

    //阻塞主线程  父线程等待子线程结束
    pthread_join(pid_read,NULL);
    pthread_join(pid_write,NULL);

    printf("释放资源\n");
    close(clientfd);
    close(sockfd);


    return 0;
}