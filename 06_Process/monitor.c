#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#define PROGRAM1 "./hello"

pid_t pid;

void start_program(const char *program)
{
    pid = fork();
    if (pid == 0) {
        // 子进程
        execl(program, program, (char *)NULL);
        perror("execl error"); // 只有 execl 失败才会执行
        exit(1);
    } else if (pid < 0) {
        perror("fork error");
        exit(1);
    }
}

void monitor_programs()
{
    while (1) {
        int   status;
        pid_t result;
        usleep(1000 * 1000);
        // 检查子进程是否退出
        // #include <sys/wait.h>
        // pid_t waitpid(pid_t pid, int *status, int options);
        // 父进程等待子进程终止。
        // pid：指定等待的子进程ID：
        // > 0：等待 PID 等于该值的子进程。
        // -1：等待任意子进程（等同于 wait()）。
        // status：同 wait()，存储子进程退出状态。
        // options：位掩码，常用选项：
        // 0：阻塞等待指定子进程终止。
        // WNOHANG：非阻塞模式，若无子进程终止立即返回 0。

        // 返回值：
        // > 0成功返回结束的子进程PID。
        // 0 非阻塞模式（WNOHANG）下，没有子进程退出。
        // -1错误（如无子进程、信号中断等），通过 errno 获取具体原因。

        // #include <sys/wait.h>
        // pid_t wait(int *status);
        // 等待任意一个子进程结束。
        // status：同 wait()，存储子进程退出状态。

        result = waitpid(pid, &status, WNOHANG); // 非阻塞
        if (result == 0) {
            // 子进程仍在运行
            printf("Child process is running...\n");
        } else if (result == -1) {
            // 错误, 通过errno获取具体原因
            printf("waitpid error");
            exit(1);
        } else if (result > 0) {
            // 成功返回子进程的PID
            printf("restart process\n");
            usleep(2 * 1000);
            start_program(PROGRAM1);
        }
    }
}

int main()
{
    // 启动程序
    start_program(PROGRAM1);
    printf("start %s\n", PROGRAM1);

    // 监控程序
    monitor_programs();
    return 0;
}
