#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[])
{

    pid_t cpid;
    int pipefd[2];

    //将程序传递进来的第一个命令行参数 通过管道传输给子进程
    if(2 != argc)
    {
        //不用perror()函数，因为只是参数不够
        fprint(stderr,"请填写需要传递的信息\n");
        exit(EXIT_FAILURE);

    }
    
    //创建管道
    if (-1 == pipe(pipefd) )
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    //复制父子进程
    cpid=foke();
    if (cpid==-1){
        perror("fork");
        exit(EXIT_FAILURE);
    }else if(cpid==0)
    {
        //子进程 读取管道的数据 打印到控制台
        clsose(pipefd[1]);  //关闭写端
        char str[100]={};
        char buf;
        while (read(pipefd[0],&buf,1)>0)
        {
            write(STDOUT_FILENO,&buf,1);
        }
        write(STDOUT_FILENO,"\n",1);
        close(pipefd[0]);
        _exit(EXIT_SUCCESS)；

    }else{

        //父进程 写入管道数据 提供给子进程
        close(pipefd[0]);  //关闭读端
        //写入数据
        printf("老学员%d对新学员传递信息\n",getpid());
        write(pipefd[1],argv[1],strlen(argv[1]));
        close(pipefd[1]);  //关闭写端
        waitpid(cpid,NULL,0);
    }

    return 0;
}