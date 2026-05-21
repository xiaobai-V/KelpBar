/**
 * 共享内存（Shared Memory）演示
 *
 * 系统调用：shm_open / ftruncate / mmap / munmap / shm_unlink
 *
 * 数据流：
 *   父进程 --[memcpy]--> 共享内存区 --[printf]--> 子进程
 *
 * 关键点：
 *   1. shm_open 创建的共享内存对象以 "/" 开头，不含额外路径分隔符
 *   2. mmap 将共享内存映射到进程地址空间后，即可像普通内存一样读写
 *   3. 共享内存本身不提供同步机制，本例用 sleep 简单等待（生产环境应使用信号量）
 *   4. mmap 成功后即可关闭 fd，映射仍然有效
 */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

#define SHM_PATH "/my_shm" // POSIX 共享内存名称：必须以 "/" 开头
#define SHM_LEN 4096

int main()
{
    // 清除上次残留的共享内存对象（shm_unlink 对不存在的路径不会报错）
    shm_unlink(SHM_PATH);

    // 创建共享内存对象，返回文件描述符
    int shm_fd = shm_open(SHM_PATH, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1)
    {
        perror("shm_open");
        return 1;
    }

    // 设置共享内存大小（新创建的共享内存大小为 0，必须 ftruncate）
    if (ftruncate(shm_fd, SHM_LEN) == -1)
    {
        perror("ftruncate");
        close(shm_fd);
        return 1;
    }

    // 将共享内存映射到进程地址空间
    // MAP_SHARED：修改对其他映射同一对象的进程可见
    char *shm = mmap(NULL, SHM_LEN, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shm == MAP_FAILED)
    {
        perror("mmap");
        close(shm_fd);
        return 1;
    }
    // 映射建立后关闭 fd，映射仍然有效（mmap 内部持有引用）
    close(shm_fd);

    int pid = fork();
    if (pid == -1)
    {
        perror("fork失败");
        munmap(shm, SHM_LEN);
        return 1;
    }
    else if (pid == 0)
    {
        // --- 子进程：读取共享内存 ---
        printf("我是子进程%d, 我的父进程是%d\n", getpid(), getppid());

        // 共享内存无内置同步机制，此处用 sleep 等待父进程写入
        // 生产环境应使用信号量（sem_open/sem_wait/sem_post）
        sleep(1);

        printf("直接从共享内存中读取数据：%s\n", shm);

        munmap(shm, SHM_LEN);
        _exit(0);
    }
    else
    {
        // --- 父进程：写入共享内存 ---
        printf("我是父进程%d, 有一个子进程%d\n", getpid(), pid);

        // 共享内存写入与普通内存写入无异
        const char *msg = "mmap共享内存数据";
        memcpy(shm, msg, strlen(msg) + 1); // +1 包含 '\0'

        int status;
        waitpid(pid, &status, 0);
        printf("Child exited with status %d\n", WEXITSTATUS(status));

        // 父进程负责清理共享内存资源
        munmap(shm, SHM_LEN);
        shm_unlink(SHM_PATH);
    }

    return 0;
}