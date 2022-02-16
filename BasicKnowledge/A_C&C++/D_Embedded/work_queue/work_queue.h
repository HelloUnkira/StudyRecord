#ifndef IDO_WORKSET_H
#define IDO_WORKSET_H
//C std lib
#include <stddef.h>
#include <stdint.h>

/* 一份工作: */
typedef struct sys_work_body {
    struct sys_work_body *next;
    /* 工作执行的类型(1~n) */
    uint8_t  type;
    /* 工作支持的执行处理类型 */
    /* 目前只支持俩类工作,其余的有待后续添加 */
    union {
        struct {    /* 工作类型1:无参类型 */
            void (*work)(void);
        } work1;
        struct {    /* 工作类型2:单参类型 */
            void (*work)(void *data);
            void  *data;
        } work2;
        struct {    /* 工作类型3:合参类型 */
            void (*work)(void *data, uint32_t size);
            void  *data;
            uint32_t size;
        } work3;
    };
	/* 有些工作在同一时间段内稳定只存在一个 */
    /* 它可能需要别的参数,去确认这份工作是否还能继续使用 */
    uint32_t static_body:1; /* 静态生成的工作 */
    uint32_t static_data:1; /* 静态生成的数据 */
} sys_work_t;

/* 工作队列 */
typedef struct sys_workset_body {
    /* 工作集合 */
    sys_work_t *head;/* 工作头节点 */
    sys_work_t *tail;/* 工作尾节点 */
} sys_workset_t;

/*@prief 为工作队列就绪一份工作
 *@param[in]    workset 工作队列
 *@param[in]    work    一份工作
 */
void sys_workset_ready_work(sys_workset_t *workset, sys_work_t *work);

/*@prief 为工作队列执行一份工作
 *@param[in]    workset 工作队列
 *@param[in]    work    一份工作
 *@retval       执行完毕的工作,其数据实体如何处理???
 *              如果工作队列没有工作,返还NULL
 */
sys_work_t * sys_workset_exec_work(sys_workset_t *workset);

#endif
