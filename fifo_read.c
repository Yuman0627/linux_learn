#include <stdlib.h>
#include <string.h>
#include <errno.h>

int main(int argc, char *argv[])
{

    int fd;
    // 创建管道
    char *pipe_path="temp/myfifo";
    // if ( mkfifo(pipe_path, 0664) !=0 )
    // {
    //     perror("mkfifo");
    //     exit(EXIT_FAILURE);
    // }
    
    // 打开管道，读文件，将数据写入控制台中
    char buf[100];
    ssize_t read_num;

    fd=open(pipe_path,O_RDONLY);
    if ( fd < 0 )
    {
        perror("open");
        exit(EXIT_FAILURE);
    }

    if ( read_num = read(fd,buf,100)>0)  //读到了数据
    {
        write(STDOUT_FILENO,buf,read_num);
    }

    if(read_num==0)
    {
        perror("read");
        close(fd);
        exit(EXIT_FAILURE);
    }
    printf("接收管道数据成功\n");
    close(fd);
    return 0;
}