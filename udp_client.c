#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdio.h>
#include<stdlib.h>
#include<string.h>
#include <arpa/inet.h>
#include <unistd.h>
#define handle_error(cmd, result) \
    if (result < 0)                \
    {                               \
        perror(cmd);                \
        return -1;                  \
    }   


int main (int argc, char *argv[])
{

    //使用UDP协议完成客户端和服务端的通讯
    //EOF作为关闭的信号
    int sockfd,temp_result;
    struct socket_in server_addr,client_addr;
    char *buf=malloc(1024);

    //清空
    memset(&server_addr,0,sizeof(server_addr));
    memset(&client_addr,0,sizeof(client_addr));
    
    //填写服务端地址，不填，主要用于接收的
    server_addr.sin_family=AF_INET;
    //填写ip地址 0.0.0.0
    server_add.sin_addr.s_addr=htol(INADDR_ANY);
    //填写端口号
    server_addr.sin_port=htons(6666);

    //UDP编程流程
    //1.socket()
    socket(AF_INET,SOCK_DGRAM,0);
    handle_error("socket",sockfd);

    //2.客户端不需要绑定地址
    socklen_t server_len=sizeof(server_addr);
    socklen_t client_len=sizeof(client_addr);
    

    //3.直接就可以收发数据
    do{

        //发数据  发从控制台接收到的数据
        printf("请输入数据：\n");

        //从控制台读取数据
        int buf_len=read(STDIN_FILENO,buf,1024);
        temp_result=sendto(sockfd,buf,buf_len,0,(struct sockaddr *)&server_addr,server_len);
        handle_error("sendto",temp_result);

        //清空buf
        memset(buf,0,1024);

        //收服务端的数据
        temp_result=recvfrom(sockfd,buf,1024,0,(struct sockaddr *)&server_addr,&server_len);
        handle_error("recvfrom",temp_result);

        if (strncmp(buf,"EOF",3)!=0)        
        {
            printf("收到服务器%s %d 返回的数据：%s\n",inet_ntoa(server_addr),ntohs(server_addr.sin_port),buf);
        }

    }while(strncmp(buf,"EOF",3)!=0);

    free(buf);

    return 0;
}
