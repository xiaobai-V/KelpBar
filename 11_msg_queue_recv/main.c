#include <fcntl.h>
#include <mqueue.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

#define MSG_QUEUE_NAME "/my_msg_queue" // POSIX 消息队列名称：必须以 "/" 开头
#define MAX_MESSAGES   10
#define MAX_MSG_SIZE   256
#define MSG_PRIORITY   1

// 必须与发送方相同的结构体定义
typedef struct {
    int   sensor_id;
    float temperature;
    char  timestamp[20];
} temp_sensor_data_t;

int main()
{
    // 1. 打开消息队列（只读模式）
    mqd_t mq = mq_open(MSG_QUEUE_NAME, O_CREAT | O_RDONLY, 0666, NULL);
    if (mq == (mqd_t)-1) {
        printf("mq_open error");
        return 1;
    }

    // 2. 获取队列属性（用于确定消息大小）
    struct mq_attr attr;
    mq_getattr(mq, &attr);
    printf("Queue max message size: %ld\n", attr.mq_msgsize);

    // 3. 接收消息
    temp_sensor_data_t received_data;
    ssize_t            bytes_read = mq_receive(mq, (char *)&received_data, attr.mq_msgsize, NULL);
    if (bytes_read == -1) {
        printf("mq_receive  error");
        mq_close(mq);
        return 1;
    }

    printf("Received: sensor_id=%d, temp=%.1f, time=%s\n",
           received_data.sensor_id,
           received_data.temperature,
           received_data.timestamp);

    // 4. 关闭并删除队列（避免资源泄漏）
    mq_close(mq);
    mq_unlink(MSG_QUEUE_NAME);
    return 0;
}
