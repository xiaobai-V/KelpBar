#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

int main()
{
    int pipefd[2]; // pipefd[0] 读 pipefd[1] 写

    if (pipe(pipefd) == -1) {
        perror("创建匿名管道失败");
        return 1;
    }

    int pid = fork();
    if (pid == -1) {
        perror("fork失败");
        return 1;
    } else if (pid == 0) {
        // 子进程
        printf("我是子进程%d, 我的父进程是%d\n", getpid(), getppid());
        close(pipefd[1]); // 关闭写
        char    read_buf[128] = {0};
        ssize_t bytes_read    = read(pipefd[0], read_buf, sizeof(read_buf));
        if (bytes_read == -1) {
            perror("read");
            close(pipefd[0]);
            _exit(1);
        } else if (bytes_read == 0) {
            printf("子进程：管道已关闭，无数据\n");
        } else {
            printf("子进程读到了%zd个数据：%s\n", bytes_read, read_buf);
        }
        close(pipefd[0]); // 关闭读
        _exit(0);
    } else {
        // 父进程
        printf("我是父进程%d, 有一个子进程%d\n", getpid(), pid);

        // 父进程发送数据
        close(pipefd[0]); // 关闭读端
        char    message[]   = "hello world!\n";
        ssize_t bytes_write = write(pipefd[1], message, strlen(message) + 1);
        if (bytes_write == -1)
            perror("write");
        else
            printf("父进程发送了%zd个数据\n", bytes_write);

        close(pipefd[1]);
        int status;
        waitpid(pid, &status, 0);
        printf("Child exited with status %d\n", WEXITSTATUS(status));
    }

    return 0;
}
