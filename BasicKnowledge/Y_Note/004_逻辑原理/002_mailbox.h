#ifndef MAILBOX
#define MAILBOX

struct mailbox_message {
	//下一个邮箱的消息
    struct mailbox_message * next;

    int size;//邮箱尺寸
    void *xfer_data;//消息数据缓冲区
};

struct mailbox {
    struct mailbox_message *queue_head;
    struct mailbox_message *queue_tail;    
    //很显然该数据结构还需要一个同步原语(锁)保护
} mbox;

//初始化一个邮箱
void mailbox_init(struct mailbox *instance)
{
    //动态或者静态创建并初始化该邮箱队列
    instance->queue_head = NULL;
    instance->queue_tail = NULL;
}

//发送一个数据到邮箱(根据实际的情况选择性设计参数)
void mailbox_message_send(struct mailbox *mbox, \
    struct mailbox_message *mbox_msg, void *buffer, int size)
{
    mbox_msg->xfer_data = buffer;
    mbox_msg->size = size;
    //为邮箱上一个锁

    //该邮箱消息插入到邮箱传输队列尾
    if (mbox->queue_head == NULL) {
        mbox->queue_head = mbox_msg;
        mbox->queue_tail = mbox_msg;
    } else {
        mbox->queue_tail->next = mbox_msg;
        mbox->queue_tail = mbox_msg;
    }
    
    //去掉邮箱上的锁    
}

//从邮箱接收一个数据
void mailbox_message_receive(struct mailbox *mbox, \
    struct mailbox_message **mbox_msg, void **buffer, int *size)
{
    //为邮箱上一个锁
    
    //查看邮箱是否存在邮件
    if (mbox->queue_head != NULL) {
        *mbox_msg = mbox->queue_head;
        *buffer = *mbox_msg->xfer_data;
        *size = *mbox_msg->size;
    
        if ((mbox->queue_head = mbox->queue_head->next) == NULL) {
            mbox->queue_tail = NULL;
        }
    }
    
    //为邮箱去一个锁
}

//可以使用各种同步原语进行实际的定制化
//使邮箱能完成同步异步传输等等

#endif
