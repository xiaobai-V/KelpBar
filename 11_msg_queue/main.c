/**
 * @brief 消息队列（Message Queue）演示
 *
 * @note 系统调用：mq_open / mq_receive / mq_close / mq_unlink
 *
 * @note 数据流：
 *   父进程 --> 消息队列 --> 子进程
 *
 * @note 关键点：
 *   1. 消息队列是命名的，需要在创建时指定名称。
 *   2. 消息队列是阻塞的，当队列满时写入，当队列空时读取。
 *   3. 消息队列的属性（如最大消息数、最大消息大小）在创建时指定。
 *
 */

#include <fcntl.h>
#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

#define MSG_QUEUE_NAME "/my_msg_queue" // POSIX 消息队列名称：必须以 "/" 开头
#define MAX_MESSAGES   10
#define MAX_MSG_SIZE   256
#define MSG_PRIORITY   1

int main()
{
    mqd_t          mq;
    struct mq_attr attr;

    // 设置消息队列属性
    attr.mq_flags   = 0; // 队列满时阻塞写入，队列空时阻塞读取
    attr.mq_maxmsg  = MAX_MESSAGES;
    attr.mq_msgsize = MAX_MSG_SIZE;
    attr.mq_curmsgs = 0;

    // 删除已经已经存在的消息队列对象，避免报错
    mq_unlink(MSG_QUEUE_NAME);

    mq = mq_open(MSG_QUEUE_NAME, O_CREAT | O_RDWR, 0666, &attr);
    if (mq == (mqd_t)-1) {
        perror("mq_open");
        return 1;
    }

    int pid = fork();
    if (pid == -1) {
        perror("fork");
        mq_close(mq);
        return 1;
    } else if (pid == 0) {
        // --- 子进程：读取消息队列 ---
        printf("我是子进程%d, 我的父进程是%d\n", getpid(), getppid());

        char msg[MAX_MSG_SIZE];
        if (mq_receive(mq, msg, sizeof(msg), NULL) == -1) { // 阻塞读取消息
            perror("mq_receive");
            mq_close(mq);
            return 1;
        }
        printf("从消息队列读取：%s\n", msg);

        _exit(0); // 子进程退出用 _exit，不会调用清理函数
    } else {
        // --- 父进程：写入消息队列 ---
        printf("我是父进程%d, 有一个子进程%d\n", getpid(), pid);

        const char *msg = "Hello from parent process";
        printf("写入消息队列：%s\n", msg);
        if (mq_send(mq, msg, strlen(msg) + 1, MSG_PRIORITY) == -1) {
            perror("mq_send");
            mq_close(mq);
            mq_unlink(MSG_QUEUE_NAME); // 删除消息队列对象
            return 1;
        }

        int status;
        waitpid(pid, &status, 0);
        printf("Child exited with status %d\n", WEXITSTATUS(status));
    }

    mq_close(mq);              // 关闭消息队列
    mq_unlink(MSG_QUEUE_NAME); // 删除消息队列对象

    return 0;
}
