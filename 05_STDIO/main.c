#include <stdio.h> // 包含标准输入输出函数
// #include <fcntl.h>  // 包含 open() 函数的声明
// #include <unistd.h> // 包含 close() 函数的声明
#include <string.h>

int main()
{
    char write_buf[] = "Hello world!\n";
    char read_buf[256];
    FILE *fp = fopen("example.txt", "w+");
    if (fp == NULL)
    {
        printf("打开文件失败\n");
        return 1;
    }
    // 写入文件
    // size_t blocks_write = fwrite(write_buf, sizeof(char), sizeof(write_buf), fp);
    size_t len = strlen(write_buf);
    size_t blocks_write = fwrite(write_buf, sizeof(char), len, fp);
    if (blocks_write != len)
    {
        printf("期望写入%zu个数据块，实际写入%zu个数据块\n", len, blocks_write);
        fclose(fp);
        return 1;
    }
    else
    {
        printf("成功写入 %zu 个数据块\n", blocks_write);
    }

    // 定位光标

    if (fseek(fp, 0, SEEK_SET) != 0)
    {
        printf("充值文件指针失败\n");
        fclose(fp);
        return 1;
    }
    // 读取文件
    size_t bytes_read = fread(read_buf, sizeof(char), sizeof(read_buf), fp);
    if (bytes_read > 0)
    {
        read_buf[bytes_read] = '\0'; // 添加字符串结束符
        printf("成功读取到 %zu 字节：%s\n", bytes_read, read_buf);
    }
    else
    {
        printf("读取文件失败\n");
        fclose(fp);
        return 1;
    }

    // 关闭文件
    if (fclose(fp) != 0)
    {
        printf("关闭文件失败\n");
    }

    return 0;
}