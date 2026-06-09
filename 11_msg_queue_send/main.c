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

typedef struct {
    int   sensor_id;
    float temperature;
    char  timestamp[20];
} temp_sensor_data_t;

int main()
{
    mqd_t          mq;
    struct mq_attr attr;

    // 创建消息队列
    mq = mq_open(MSG_QUEUE_NAME, O_CREAT | O_RDWR, 0655, NULL);
    if (mq == (mqd_t)-1) {
        perror("mq_open");
        return 1;
    }

    // 准备结构化数据
    temp_sensor_data_t data = {
        .sensor_id   = 1001,
        .temperature = 25.3,
        .timestamp   = "13:14:00",
    };

    // 发送数据
    if (mq_send(mq, (const char *)&data, sizeof(data), 1) == -1) {
        perror("mq_send");
        mq_close(mq);
        return 1;
    }

    printf("[SEND]: sersor_id = %d, temp = %.1f\n", data.sensor_id, data.temperature);

    // 关闭消息队列
    mq_close(mq);

    return 0;
}
