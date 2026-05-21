#include <stdio.h>  // 包含标准输入输出函数
#include <fcntl.h>  // 包含 open() 函数的声明
#include <unistd.h> // 包含 close() 函数的声明
#include <string.h>

// 头文件：
// #include <unistd.h>
// ssize_t write(int fd, const void *buf, size_t count);

// 参数说明
// fd：文件描述符
// 表示要写入的文件（如 open() 返回的值）。
// buf：指向内存缓冲区的指针
// 要写入的数据。
// count：请求写入的字节数。

// 返回值
// 成功：返回实际写入的字节数。
// 失败：返回 -1

// FYI:
// size_t
// 定义：unsigned long 或 unsigned long long 的类型别名（具体取决于平台，32/64 位）。
// ssize_t
// 定义：signed long 或 signed long long 的类型别名（与 size_t 对应的有符号版本）

int main()
{
    int fd;
    ssize_t bytes_written;
    ssize_t bytes_read;
    char write_buf[] = "Hello, World!\n";
    char read_buf[1024];
    // 打开文件（如果不存在则创建，设置读写权限）
    fd = open("example.txt", O_RDWR | O_CREAT, 0644);

    if (fd == -1)
    {
        printf("打开文件失败");
        return 1;
    }

    // 使用文件描述符进行读写操作...
    bytes_written = write(fd, write_buf, strlen(write_buf));
    if (bytes_written == -1)
    {
        printf("写入文件失败");
        close(fd);
        return 1;
    }
    // %zd 格式化输出 ssize_t 类型的变量，确保在 32/64 位平台上的正确显示（）
    // %zu 格式化输出 size_t 类型的变量，确保在 32/64 位平台上的正确显示（）
    //  long long 类型
    printf("写入 %zd 字节到文件 example.txt\n", bytes_written);
    // 注意：写入后，文件指针会自动移动到写入位置的下一个字节，
    // 所以，如果要读取写入的内容，需要先移动文件指针到文件开头
    // 可以使用 lseek() 函数来实现文件指针的移动
    // 头文件：
    // #include <unistd.h>
    // off_t lseek(int fd, off_t offset, int whence);

    // 参数说明
    // fd：文件描述符
    // 表示要操作的文件。
    // offset：偏移量（字节数）
    // 可为正（向后移动）、负（向前移动）或 0（不移动）。
    // whence：
    // 基准位置，取值为以下三个宏：
    // SEEK_SET：从文件开头计算偏移量。
    // SEEK_CUR：从当前位置计算偏移量。
    // SEEK_END：从文件末尾计算偏移量（offset 可为负数，表示倒数位置）。

    // 返回值
    // 成功：返回新的文件偏移量（从文件开头算起的字节数）。
    // 失败：返回 -1。

    if (lseek(fd, 0, SEEK_SET) == -1) // 移动文件指针到文件开头，否则读取到的是写入后的内容
    {
        printf("移动文件指针到文件开头失败");
        close(fd);
        return 1;
    }

    bytes_read = read(fd, read_buf, sizeof(read_buf));
    if (bytes_read == -1)
    {
        printf("读取文件失败");
        close(fd);
        return 1;
    }
    printf("读取 %zd 字节从文件 example.txt\n", bytes_read);
    printf("读取到的内容：%s", read_buf);

    // 关闭文件
    close(fd);
    return 0;
}