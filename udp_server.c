#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdio.h>
#include<stdlib.h>
#include<string.h>
#include <arpa/inet.h>
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

    //2.bind()绑定地址
    socklen_t server_len=sizeof(server_addr);
    socklen_t client_len=sizeof(client_addr);
    temp_result=bind(sockfd,(struct sockaddr *)&server_addr,server_len);
    handle_error("bind",temp_result);

    //3.直接就可以收发数据
    do{

        //接收数据
        memset(buf,0,1024);
        temp_result=recvfrom(sockfd,buf,1024,0,(struct sockaddr *)&client_addr,&client_len);
        handle_error("recvfrom",temp_result);

        if (strncmp(buf,"EOF",3)!=0)
        {
            printf("接收到客户端%s  %d信息：%s\n",inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port),buf);
            strcpy(buf,"OK\n");
        }else{
            printf("收到结束信息 准备关闭\n");
        }

        //回复数据
        sendto(sockfd,buf,4,0,(struct sockaddr *)&client_addr,client_len);
        handle_error("sendto",temp_result);

    }while(strncmp(buf,"EOF",3)!=0);

    free(buf);

    return 0;
}
