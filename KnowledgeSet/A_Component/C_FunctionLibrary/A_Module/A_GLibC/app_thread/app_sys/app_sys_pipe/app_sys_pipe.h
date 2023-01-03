#ifndef APP_SYS_PIPE_H
#define APP_SYS_PIPE_H

typedef struct {
    void    *near;          /* 管道是队列 */
    uint32_t send_tid;      /* 发送者线程ID */
    uint32_t recv_tid;      /* 接收者线程ID */
    uint32_t module;        /* 接收者线程模组ID */
    uint32_t event;         /* 接收者线程模组事件 */
    uint32_t size;          /* 协议数据流大小 */
    void    *data;          /* 协议数据流(浅拷贝) */
} app_package_t;

typedef struct {
    app_package_t *head;    /* 管道头 */
    app_package_t *tail;    /* 管道尾 */
    unsigned int   number;  /* 管道项数 */
    app_mutex_t    mutex;   /* 管道资源保护 */
} app_pipe_t;

/*@brief        初始化管道
 *@param[in]    pipe 管道实例
 */
static inline void app_sys_pipe_ready(app_pipe_t *pipe)
{
    pipe->head = NULL;
    pipe->tail = NULL;
    pipe->number = 0;
    app_mutex_process(&pipe->mutex);
}

/*@brief        获取管道资源包数量
 *@param[in]    pipe 管道实例
 *@retval       管道资源包数量
 */
uint32_t app_sys_pipe_package_num(app_pipe_t *pipe);

/*@brief        交付一个包给管道
 *@param[in]    pipe     管道实例
 *@param[in]    package  事件资源包(栈资源,非堆资源或静态资源)
 */
void app_sys_pipe_give(app_pipe_t *pipe, app_package_t *package);

/*@brief        从管道提取一个包
 *@param[in]    pipe     管道实例
 *@param[out]   package  事件资源包(栈资源,非堆资源或静态资源)
 */
void app_sys_pipe_take(app_pipe_t *pipe, app_package_t *package);

#endif