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
 * @brief: 09_Fifo_Read
 * @author: hjy
 * @date: 2023-08-20
 * @version: 0.1
 * @description: 09_Fifo_Read，测试fifo在单独两个进程间通信，这是读取端
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
    char buf[128]; // 接收缓冲区
    // 读取端负责创建fifo，如果已存在则忽略
    if (mkfifo(MY_FIFO_PATH, 0666) == -1 && errno != EEXIST) {
        perror("mkfifo");
        return 1;
    }
    printf("start read data from fifo\n");

    // 打开fifo，阻塞等待写入端打开
    int fd = open(MY_FIFO_PATH, O_RDONLY);
    if (fd == -1) {
        perror("open my_fifo error");
        return 1;
    }

    while (1) {
        // 阻塞读取fifo，没有数据时自动阻塞等待
        int bytes_read = read(fd, buf, sizeof(buf) - 1);
        if (bytes_read == -1) {
            perror("read my_fifo error");
            break;
        } else if (bytes_read == 0) {
            printf("发送端已经关闭fifo\n");
            break;
        }
        buf[bytes_read] = '\0';
        printf("read %s\n", buf);
    }

    close(fd);
    // 读取端不负责删除fifo，由写入端删除

    return 0;
}
