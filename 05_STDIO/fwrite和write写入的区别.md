write:
当调用write时，它会把数据从用户空间复制到内核空间的缓冲区（页缓存）。
可以通过 fsync 强制数据写入磁盘。
fwrite:
使用标准 I/O 库的缓冲机制。数据可能先写入用户空间缓冲区，然后再调用内核IO写入内核空间。
可以通过 fflush 强制刷新缓冲区进行系统调用。
```c
#include <unistd.h>
int fsync(int fd);
作用：手动强制写入磁盘
参数
fd：文件描述符。
返回值
成功时返回 0。

#include <stdio.h>
int fflush(FILE *stream);
作用：手动强制写入系统
参数
stream: 指向 FILE 结构的指针。
返回值
成功：返回 0。
```
所以，对于一些关键数据，为了避免丢失情况，我们应该通过fsync函数手动强制写入磁盘
```c
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

int main() {
    int fd = open("example.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        printf("open error");
        return 1;
    }
    const char *message = "Hello, World!\n";
    ssize_t bytes_written = write(fd, message, strlen(message));
    if (bytes_written == -1) {
        printf("write error");
        close(fd);
        return 1;
    }

    // 强制将数据同步到磁盘
    if (fsync(fd) == -1) {
        printf("fsync error");
        close(fd);
        return 1;
    }
    close(fd);
    return 0;
}
```
同理，对于使用fwrite的写入，先执行fflush，再执行fsync
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>  // for fsync
#include <fcntl.h>   // for open
#include <sys/types.h>
#include <sys/stat.h>

int main() {
    const char *filename = "example.txt";
    const char *message = "Hello, World!\n";
    size_t message_length = strlen(message);

    // 打开文件以写模式
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("fopen");
        return 1;
    }

    // 使用 fwrite 将字符串写入文件
    size_t bytes_written = fwrite(message, 1, message_length, file);
    if (bytes_written != message_length) {
        perror("fwrite");
        fclose(file);
        return 1;
    }

    // 刷新文件缓冲区，确保数据写入文件
    if (fflush(file) != 0) {
        perror("fflush");
        fclose(file);
        return 1;
    }
    printf("数据已写入系统.\n");

    // 获取文件描述符
    int fd = fileno(file);
    if (fd == -1) {
        perror("fileno");
        fclose(file);
        return 1;
    }

    // 使用 fsync 同步文件数据到磁盘
    if (fsync(fd) != 0) {
        perror("fsync");
        fclose(file);
        return 1;
    }
    // 关闭文件
    fclose(file);
    printf("数据已写入磁盘.\n");
    return 0;
}
```