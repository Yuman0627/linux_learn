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
    
void *read_from_client_then_write(void *arg)
{
    //使用recv接收客户端发送的数据 打印到控制台
    char * read_buf=NULL;
    char * write_buf=NULL;

    int clientfd=*(int *)arg;
    read_buf=malloc(sizeof(char) * 1024);
    ssize_t read_len=0;

    write_buf=malloc(sizeof(char) * 1024);
    ssize_t write_len=0;

    if (!read_buf)
    {
        perror("malloc read");
        return NULL;
    }
    if (!write_buf)
    {
        perror("malloc write");
        return NULL;
    }
    
    while(read_len=recv(clientfd,read_buf,1024,0))
    {        
        if (read_len<0)
        {
            perror("recv");
        }
        //接收数据,打印到控制台
        printf("从%d客户端收到数据%s\n",clientfd,read_buf);

        //把“收到”的消息写到缓存
        strcpy(write_buf,"收到\n");
        write_len=send(client_fd,write_buf,1024,0);
        if (write_len<0)
        {
            perror("send");
        }

    }
    //当客户端ctrl+d时会退出
    //shutdown(clientfd,SHUT_RDWR);

    printf("客户端%d已退出\n",clientfd);
    free(read_buf);
    free(write_buf);

    return NULL;

}

int main(int argc, char *argv[])
{
    int sockfd,temp_result;

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
    socklen_t cliaddr_len=sizeof(client_addr);
    while(1)
    {
        int clientfd=accept(sockfd,(struct sockaddr *)&client_addr,&cliaddr_len);
        pthread_t pid_read_write;

        handle_error("accept",clientfd);
        printf("与客户端%s %d 建立连接%d\n",inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port),clientfd);
        //和每一个客户端使用一个线程交互 
        //把客户端发送的信息打印到控制台 回复收到
        if (pthread_create(&pid_read_write,NULL,read_from_client_then_write,(void *)&clientfd) )
        {
            perror("pthread_create");
        }
        //需要等待线程结束 但是不能挂起等待
        pthread_detach(pid_read_write);
     
    }
    
    printf("释放资源\n");
    close(sockfd);


    return 0;
}