#include <stdio.h>
#include <string.h>

int main()
{
    char write_buf[] = "Hello world!\n";

    // ===== 阶段1：用 sizeof 写入（包含 \0）=====
    FILE *fp = fopen("test_binary.txt", "w+");
    if (fp == NULL)
    {
        printf("打开文件失败\n");
        return 1;
    }

    size_t written = fwrite(write_buf, sizeof(char), sizeof(write_buf), fp);
    printf("[sizeof版] 写入字节数: %zu (strlen=%zu, sizeof=%zu)\n",
           written, strlen(write_buf), sizeof(write_buf));

    // 用 fread 读回来验证
    rewind(fp);
    char read_buf[32] = {0};
    size_t read_count = fread(read_buf, sizeof(char), sizeof(write_buf), fp);
    printf("[fread读取] 读到字节数: %zu\n", read_count);
    printf("[fread读取] 内容: \"");
    for (size_t i = 0; i < read_count; i++)
    {
        if (read_buf[i] == '\0')
            printf("\\0");
        else if (read_buf[i] == '\n')
            printf("\\n");
        else
            printf("%c", read_buf[i]);
    }
    printf("\"\n");

    fclose(fp);

    // ===== 阶段2：fwrite 返回值测试 =====
    printf("\n[返回值测试] fwrite 返回类型是 size_t (无符号)\n");
    printf("[返回值测试] (size_t)-1 = %zu\n", (size_t)-1);
    printf("[返回值测试] 所以 blocks_write == -1 永远为 false\n");

    return 0;
}
