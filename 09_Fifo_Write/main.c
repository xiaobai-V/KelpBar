#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h> // mkfifo
#include <sys/stat.h>
#include <sys/types.h> // mkfifo
#include <sys/wait.h>
#include <unistd.h>

#define MY_FIFO_PATH "/tmp/my_fifo"

/*
 * @brief: 09_Fifo_Write
 * @author: hjy
 * @date: 2023-08-20
 * @version: 0.1
 * @description: 09_Fifo_Write，测试fifo在单独两个进程间通信，这是写入端
 */

// 问题：
// Q: 谁来负责创建fifo?
// A: 读取端负责创建，因为读取端 open(O_RDONLY) 会阻塞等待写入端
// Q: 谁来负责删除fifo?
// A: 写入端负责删除，因为写入端清楚何时不再发送数据
// Q: 读取和写入fifo那个先运行？
// A: 都可以。读取端先运行会阻塞在 open 等待写入端；写入端先运行会阻塞在 open 等待读取端

int main()
{
    // 写入端也可以容忍读取端未启动的情况，尝试创建 fifo（如果已存在则忽略）
    if (mkfifo(MY_FIFO_PATH, 0666) == -1 && errno != EEXIST) {
        perror("mkfifo");
        return 1;
    }

    printf("start write data to fifo\n");
    const char *msg = "hello world";
    // 阻塞等待读取端打开
    int fd = open(MY_FIFO_PATH, O_WRONLY);
    if (fd == -1) {
        perror("open");
        return 1;
    }

    size_t count = 0;
    while (1) {
        printf("write data %zu: %s\n", count++, msg);
        // 每隔1s写入fifo
        ssize_t ret = write(fd, msg, strlen(msg));
        if (ret == -1) {
            perror("write");
            break;
        }
        sleep(1);
    }

    close(fd);
    unlink(MY_FIFO_PATH); // 写入端负责删除fifo文件
    return 0;
}
