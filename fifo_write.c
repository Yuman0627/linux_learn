#include <stdlib.h>
#include <string.h>
#include <errno.h>

int main(int argc, char *argv[])
{

    int fd;
    // 创建管道
    char *pipe_path="temp/myfifo";
    if ( mkfifo(pipe_path, 0664) !=0 )
    {
        perror("mkfifo");
        if (errno != EEXIST)
        {
            exit(EXIT_FAILURE);
        }
        
    }
    
    // 打开管道，读控制台，将数据写入文件中
    char buf[100];
    ssize_t read_num;

    fd=open(pipe_path,O_WRONLY);
    if ( fd < 0 )
    {
        perror("open");
        exit(EXIT_FAILURE);
    }

    if ( read_num = read(STDIN_FILENO,buf,100)>0)  //读到了数据
    {
        write(fd,buf,read_num);
    }

    if(read_num==0)
    {
        perror("read");
        close(fd);
        exit(EXIT_FAILURE);
    }
    printf("发送数据到管道成功\n");
    close(fd);

    // 删除管道
    if (unlink(pipe_path)==-1)
    {
        perror("unlink");
        exit(EXIT_FAILURE);
    }
    return 0;
}