#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <sys/socket.h>
#include<sys/un.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>

#define S0CKET_PATH "unix_domain.socket"
#define SERVER MODE 1
#define CLIENT MODE 2
#define BUF LEN 1024
static struct sockaddr_un socket_addr;static char *buf;
void handle_error(char *err_msg)
{
    perror(err_msg);
    unlink(SOCKET_PATH);
    exit(-1);
}

void server_mode(int fd)
{
    //服务端收消息 收到EOF表示结束
    int client_fd;
    struct sockaddr_un client_addr;
    //1.绑定
    if (bind(sockfd,(struct sockaddr * )&socket_addr,sizeof(socket_addr)) < 0)
    {
        handle_error("bind");
    }
    //2.挂起监听
    if (listen(sockfd,128) < 0)
    {
        handle_error("listen");
    }
    //3.获取连接
    socklen_t client_addr_len = sizeof(client_addr);
    if ( client fd = accept(sockfd,(struct sockaddr *)&client addr, &client addr len ) )
    {
        handle_error("accept");
    }
    printf("接收到客户端的连接\n");

    do{

        //清空buf
        memset(buf,0,1024);

        //收服务端的数据
        msg_len=recv(client_fd,buf,BUF_LEN,0);
        handle_error("recvfrom",temp_result);

        if (strncmp(buf,"EOF",3)!=0)        
        {
            printf("收到服务器数据：%s\n",buf);
            strcpy(buf,"ok\n");
                   
        }else{
            printf("收到EOF，停止接收数据\n");
            
        }
        //回复ok
        send(client_fd,buf,strlen(buf),0);

    }while(strncmp(buf,"EOF",3)!=0);

    unlink(SOCKET_PATH);
}

void client_mode(int fd)
{
    //客户端发送消息 发送EOF表示结束
    int msg_len;
    //连接服务端
    if ( connect(sockfd,(struct sockaddr * )&socket addr,sizeof(socket addr) ) < 0)
    {
        handle_error("connect");
    }
    do{
        printf("请输入要发送的数据\n");

        msg_len = read(STDIN_FILENO,buf,BUF_LEN);
        
        if(send(sockfd,buf,BUF_LEN,0) < 0)
        {
            handle_error("send");
        }
        // 清空缓冲区
        memset(buf,0,1024);
        recv(sockfd,buf,1024,0);
        printf("接收到服务端回复的消息:%s",buf);
    }while (strncmp(buf,"EOF",3)!=0);

    if (shutdowm(sockfd,SHUT_RDWR)<0)
    {
        handle_error("shutdown");
    }
}
int main(int argc, char **argv)
{
    int fd,mode = 0;
    //设计结构
    //通过main方法传参的方式 启动不同的客户端和服务端
    //如果不填写参数 或者填写参数为server

    if (argc==1 || strcmp(argv[1],"server",6)==0)
    {
        mode = SERVER;
    }else if(strcmp(argv[1],"client",6)==0))
    {
        mode = CLIENT;
    }else{
        perror("参数错误");
        exit(EXIT_FAILURE);
    }

    //创建socket ipc通讯
    memset(&socket_addr,0,sizeof(struct socket_addr));
    buf=malloc(BUF_LEN);
    socket_addr.sun_family = AF_UNIX;
    strcpy(socket_addr.sun_path,SOCKET_PATH);
    fd=socket(AF_UNIX,SOCK_STREAM,0);
    if (fd==-1)
    {
        handle_error("socket");
    }

    switch(mode)
    {
        case SERVER:
            server_mode(fd);
            break;

        case CLIENT:
            client_mode(fd);
            break;

        default:
            break;
    }
    close(fd);
    free(buf);
    
    
    return 0;
}


