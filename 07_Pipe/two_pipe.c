#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main()
{
    int pipefd1[2]; // 管道1，用于从父进程到子进程的通信
    int pipefd2[2]; // 管道2，用于从子进程到父进程的通信
    pid_t pid;
    char buf[100]; // 缓冲区用于读取数据

    // 创建管道1
    if (pipe(pipefd1) == -1)
    {
        printf("pipe1 error");
        exit(1);
    }
    // 创建管道2
    if (pipe(pipefd2) == -1)
    {
        printf("pipe2 error");
        exit(1);
    }

    // 创建子进程
    pid = fork();
    if (pid == -1)
    {
        printf("fork error");
        exit(1);
    }

    if (pid == 0)
    {
        // 子进程
        close(pipefd1[1]); // 关闭管道1的写端
        close(pipefd2[0]); // 关闭管道2的读端
        // 从管道1读取数据
        ssize_t bytes_read = read(pipefd1[0], buf, sizeof(buf));
        if (bytes_read == -1)
        {
            printf("read from pipe1 error");
            exit(1);
        }
        printf("Child received from parent: %.*s\n", (int)bytes_read, buf);
        // 向管道2写入数据
        const char *msg = "Hello, Parent!";
        ssize_t bytes_written = write(pipefd2[1], msg, strlen(msg) + 1);
        if (bytes_written == -1)
        {
            printf("write to pipe2 error");
            exit(1);
        }
        close(pipefd1[0]); // 关闭管道1的读端
        close(pipefd2[1]); // 关闭管道2的写端
    }
    else
    {
        // 父进程
        close(pipefd1[0]); // 关闭管道1的读端
        close(pipefd2[1]); // 关闭管道2的写端
        // 向管道1写入数据
        const char *msg = "Hello, Child!";
        ssize_t bytes_written = write(pipefd1[1], msg, strlen(msg) + 1);
        if (bytes_written == -1)
        {
            printf("write to pipe1 error");
            exit(1);
        }
        // 从管道2读取数据
        ssize_t bytes_read = read(pipefd2[0], buf, sizeof(buf));
        if (bytes_read == -1)
        {
            printf("read from pipe2 error");
            exit(1);
        }
        printf("Parent received from child: %.*s\n", (int)bytes_read, buf);
        close(pipefd1[1]); // 关闭管道1的写端
        close(pipefd2[0]); // 关闭管道2的读端
        // 等待子进程结束
        wait(NULL);
    }
    return 0;
}