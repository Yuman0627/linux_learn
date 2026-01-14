#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h> 


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

//关闭回收所有的进程 避免僵尸进程
void zombie_dealer(int sig) 
{
    pid t pid;
    int status;
    // 一个SIGCHLD可能对应多个子进程的退出
    // 使用while循环回收所有退出的子进程，避免僵尸进程的出现
    while ((pid = waitpid(-1, &status, WNoHANG)) > 0) {
        if (WIFEXITED(status)) {
            printf("子进程:%d 以%d状态正常退出,已被回收\n",pid,WEXITSTATUS(status));
        } else if (WIFSIGNALED(status)) {
            printf("子进程:%d被%d信号杀死,已被回收\n",pid,WTERMSIG(status));
        }else {
            printf("子进程:%d因其它原因退出,已被回收\n",pid);
        }
    }
}
int main(int argc, char *argv[])
{
    int sockfd,temp_result;

    struct socket_in server_addr,client_addr;

    //注册信号处理函数 SIGCHLD
    signal(SIGCLD,zombie_dealer);


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
        
        //创建单独的子进程和连接的客户端交互
        pid_t pid=fork();
        if(pid< 0)
        {
            perror("fork");
        }else if(pid==0)
        {
            //子进程
            //关闭不会使用的sockfd
            close(sockfd);

            printf("与客户端%s %d 建立连接%d\n",inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port),clientfd);
            //从客户端读取数据 并回复
            read_from_client_then_write((void *)&clientfd);

            //关闭clientfd
            close(clientfd);

            //退出子进程
            exit(EXIT_SUCCESS);

        }else{
            //父进程
            //关闭不会使用的clientfd
            close(clientfd);


        }

     
    }
    
    printf("释放资源\n");
    if (pid>0)
    {
        close(sockfd);
    }
    


    return 0;
}