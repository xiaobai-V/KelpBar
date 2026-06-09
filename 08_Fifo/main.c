#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h> // mkfifo
#include <sys/stat.h>
#include <sys/types.h> // mkfifo
#include <sys/wait.h>
#include <unistd.h>

#define MY_FIFO_PATH "/tmp/my_fifo"

// 代码演进过程：
// 父进程用O_RDWR打开fifo导致死锁：fd = open(MY_FIFO_PATH, O_RDWR);
// 子进程末尾需要留一个数据填 '\0'：ssize_t bytes_read = read(fd, read_buf, sizeof(read_buf) - 1); // 给 \0
// 预留一个位置，防止越界 子进程的读端的缓冲区大小不够 -> 多次循环读取

int main()
{
    // ❗️多次运行的时候，删除可能已经存在的fifo
    unlink(MY_FIFO_PATH);

    if (mkfifo(MY_FIFO_PATH, 0666) == -1) {
        perror("mkfifo");
        return 1;
    }

    int pid = fork();
    if (pid == -1) {
        perror("fork失败");
        return 1;
    } else if (pid == 0) {
        // 子进程
        // sleep(1);
        printf("我是子进程%d, 我的父进程是%d\n", getpid(), getppid());

        int  fd;           // fifo的文件描述符
        char read_buf[10]; // 接收缓冲区，大小从100缩小到10
        fd = open(MY_FIFO_PATH, O_RDONLY);
        if (fd == -1) // 读取失败
        {
            perror("open");
            unlink(MY_FIFO_PATH);
            _exit(1);
        }

        // ssize_t bytes_read = read(fd, read_buf, sizeof(read_buf));
        // ssize_t bytes_read = read(fd, read_buf, sizeof(read_buf) - 1); // 给 \0 预留一个位置，防止越界
        // 读端循环读取
        ssize_t bytes_read;
        while ((bytes_read = read(fd, read_buf, sizeof(read_buf) - 1)) > 0) {
            read_buf[bytes_read] = '\0';
            printf("读取到%zd字节：%s\n", bytes_read, read_buf);
        }

        if (bytes_read == -1) // 读取失败
            perror("read");
        else if (bytes_read == 0)
            printf("没有数据可以读出\n");
        // else
        // {
        //     // ❗️读端手动加上\0
        //     read_buf[bytes_read] = '\0';
        //     printf("成功读取到%zd字节的数据：%s\n", bytes_read, read_buf);
        // }

        _exit(0);
    } else {
        // 父进程
        printf("我是父进程%d, 有一个子进程%d\n", getpid(), pid);

        // 父进程向fifo中写数据
        // 类似文件读写
        int   fd;
        char *msg = "a message from fifo\n";
        // fd = open(MY_FIFO_PATH, O_RDWR); // ❌ 错误
        fd = open(MY_FIFO_PATH, O_WRONLY);
        if (fd == -1) {
            perror("open fifo");
            unlink(MY_FIFO_PATH);
            return 1;
        }

        ssize_t bytes_write =
            write(fd, msg, strlen(msg)); // 传入长度是否需要设置为strlen(msg) + 1以把 '\0' 也写入到文件呢
        if (bytes_write == -1) {
            perror("write fifo");
            close(fd);
            unlink(MY_FIFO_PATH);
            exit(1);
        } else {
            printf("父进程%d向fifo中写入了%zd个数据\n", getpid(), bytes_write);
        }

        close(fd);

        int status;
        waitpid(pid, &status, 0);
        printf("Child exited with status %d\n", WEXITSTATUS(status));
    }

    // 只在父进程中释放一次fifo
    if (pid > 0)
        unlink(MY_FIFO_PATH);

    return 0;
}
