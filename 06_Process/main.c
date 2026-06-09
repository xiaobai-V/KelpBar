#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

int main()
{
    printf("This is a fork demo\r\n");

    int pid = fork();
    if (pid == -1) {
        perror("fork失败");
        return 1;
    } else if (pid == 0) {
        // 子进程
        printf("我是子进程%d, 我的父进程是%d\n", getpid(), getppid());
        if (execl("/bin/ls", "ls", "-l", "/home", NULL) < 0)
            perror("execl error");
        return 1;
    } else {
        // 父进程
        printf("我是父进程%d, 有一个子进程%d\n", getpid(), pid);
        int status;
        waitpid(pid, &status, 0);
        printf("Child exited with status %d\n", WEXITSTATUS(status));
    }

    return 0;
}
