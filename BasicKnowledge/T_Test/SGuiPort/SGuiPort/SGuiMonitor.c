/*实现目标:
 *一个内存分配释放的监控策略
 *通常,内存本身的分配释放策略已经高效化
 *无需额外指定额外的分配释放策略
 *可选项:块式slab快速分配策略
 *但这会进一步加重内存负担,索性忽略
 */

/* 核心唯一依赖项 */
#include "SGuiPort.h"

#if SGUI_USE_LINUX
/* 动态分配与释放 */
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

#define SGUIPORTMALLOC(size)    malloc(size)
#define SGUIPORTFREE(pointer)   free(pointer)
#define SGUIPORTPRINT(...)      printf(__VA_ARGS__)

/* 继续添加其余平台
#elif SGUI_USE_UNKNOWN
*/

#else

#error  "Cannot obtain dynamic memory policy support"
#define SGUIPORTMALLOC(size)
#define SGUIPORTFREE(pointer)
#define SGUIPORTPRINT(...)

#endif

/* 内存监控策略(精简版) */
typedef struct SimpleGuiMemoryCheckUnit {
    int8_t   file[SGUIMEMORYCHECKFILELEN];
    int32_t   line;
    void * pointer; 
    int32_t   size;
    int8_t   state;
} SGuiMemoryCheckUnit;

/* 使用一个简单开关控制记录启停 */
static bool StartOrEnd = false;

/* 启用记录 */
void SGuiMemoryCheckStart(void)
{
    SGuiMemoryCheckOptSync(true);
    StartOrEnd = true;
    SGuiMemoryCheckOptSync(false);
}

/* 停止记录 */
void SGuiMemoryCheckEnd(void)
{
    SGuiMemoryCheckOptSync(true);
    StartOrEnd = false;
    SGuiMemoryCheckOptSync(false);
}



#if SGUIMEMORYCHECK

static int32_t record_count   = 0;
static int32_t position_start = 0;
static int32_t position_end   = 0;
static SGuiMemoryCheckUnit ring_queue[SGUIMEMORYCHECKLEN] = {0};

void SGuiMemoryCheckReset(void)
{
    SGuiMemoryCheckOptSync(true);
    SGuiMemoryCheckUnit unit = {0};
    record_count   = 0;
    position_start = 0;
    position_end   = 0;
    ring_queue[0]  = unit;
    SGuiMemoryCheckOptSync(false);
}

void SGuiMemoryCheck(void)
{
    int32_t start = position_start;
    int32_t end   = position_end;
    /* 循环队列如果双重覆盖,本身没什么意义 */
    /* 需要解决这个问题只需要添加一个监测哨兵 */
    /* 每end超过start时,监测哨兵计数+1 */
    /* 每start超过end时,监测哨兵计数-1 */
    /* 根据其奇偶性即可判断正确的迭代方向 */
    /* 因为有SGUIMEMORYCHECKAUTO去限制 */
    /* 所以不会出现多次覆盖的情况 */
    /* 从start走到end就行了 */
    /* 场景1:start <= end */
    /* 场景1:start >  end */

    if (start > end) {
        while (start < SGUIMEMORYCHECKLEN) {
            if (ring_queue[start].state == 1)
                SGUIPORTPRINT("\nSGUIPORTMALLOC:");
            if (ring_queue[start].state == 0)
                SGUIPORTPRINT("\nSGUIPORTFREE:");
            SGUIPORTPRINT("\naddress:%p ", ring_queue[start].pointer);
            SGUIPORTPRINT(  "size:%d",     ring_queue[start].size);
            SGUIPORTPRINT("\nfile:%s",     ring_queue[start].file);
            SGUIPORTPRINT("\nline:%d\n",   ring_queue[start].line);
            start++;
        }
        start = 0;
    }

    if (start < end) {
        while (start < end) {
            if (ring_queue[start].state == 1)
                SGUIPORTPRINT("\nSGUIPORTMALLOC:");
            if (ring_queue[start].state == 0)
                SGUIPORTPRINT("\nSGUIPORTFREE:");
            SGUIPORTPRINT("\naddress:%p ", ring_queue[start].pointer);
            SGUIPORTPRINT(  "size:%d",     ring_queue[start].size);
            SGUIPORTPRINT("\nfile:%s",     ring_queue[start].file);
            SGUIPORTPRINT("\nline:%d\n",   ring_queue[start].line);
            start++;
        }
    }
}

void * SGuiMalloc(uint32_t size, int8_t *file, uint32_t line)
{
    SGuiMemoryCheckOptSync(true);
    /* 关闭记录时 */
    if (StartOrEnd == false)
    {
        void *pointer = SGUIPORTMALLOC(size);
        SGuiMemoryCheckOptSync(false);

        return pointer;
    }

    void *pointer = SGUIPORTMALLOC(size);
    int32_t file_length = strlen(file);
    /* 逆向截断存储 */
    while (file_length  > SGUIMEMORYCHECKFILELEN) {
           file_length -= SGUIMEMORYCHECKFILELEN;
           file        += SGUIMEMORYCHECKFILELEN;
    }
    /* 相等情况额外处理 */
    if (file_length == SGUIMEMORYCHECKFILELEN) {
        file_length -= 1;
        file        += 1;
    }
    /* 载入记录 */
    strcpy(ring_queue[position_end].file, file);
    ring_queue[position_end].line    = line;
    ring_queue[position_end].pointer = pointer;
    ring_queue[position_end].size    = size;
    ring_queue[position_end].state   = 1;
    /* 循环队列(较为廉价的实现) */
    if (position_end < position_start)
        position_start++;
        position_end++;
    if (position_end >= SGUIMEMORYCHECKLEN) {
        position_start = 1;
        position_end   = 0;
    }
    /* 自动检查 */
    if (SGUIMEMORYCHECKAUTO != 0) {
        record_count++;
        if (record_count >= SGUIMEMORYCHECKAUTO) {
            record_count -= SGUIMEMORYCHECKAUTO;
            SGuiMemoryCheck();
        }
    }

    SGuiMemoryCheckOptSync(false);
    /* 可在此处添加分配失败断言 */
    return pointer;
}

void SGuiFree(void *pointer, int8_t *file, uint32_t line)
{
    SGuiMemoryCheckOptSync(true);
    /* 关闭记录时 */
    if (StartOrEnd == false)
    {
        SGUIPORTFREE(pointer);
        SGuiMemoryCheckOptSync(false);

        return;
    }

    int32_t file_length = strlen(file);
    /* 逆向截断存储 */
    while (file_length  > SGUIMEMORYCHECKFILELEN) {
           file_length -= SGUIMEMORYCHECKFILELEN;
           file        += SGUIMEMORYCHECKFILELEN;
    }
    /* 相等情况额外处理 */
    if (file_length == SGUIMEMORYCHECKFILELEN) {
        file_length -= 1;
        file        += 1;
    }
    /* 载入记录 */
    strcpy(ring_queue[position_end].file, file);
    ring_queue[position_end].line    = line;
    ring_queue[position_end].pointer = pointer;
    ring_queue[position_end].size    = 0;
    ring_queue[position_end].state   = 0;
    /* 循环队列(较为廉价的实现) */
    if (position_end < position_start)
        position_start++;
        position_end++;
    if (position_end >= SGUIMEMORYCHECKLEN) {
        position_start = 1;
        position_end   = 0;
    }
    /* 自动检查 */
    if (SGUIMEMORYCHECKAUTO != 0) {
        record_count++;
        if (record_count >= SGUIMEMORYCHECKAUTO) {
            record_count -= SGUIMEMORYCHECKAUTO;
            SGuiMemoryCheck();
        }
    }

    /* 释放一般是不需要额外检查的 */
    /* 因为没人知道指针传入后,释放是否有效 */
    SGUIPORTFREE(pointer);

    SGuiMemoryCheckOptSync(false);
}

/* 继续添加其余平台
#elif SGUI_USE_UNKNOWN
*/
#else

/* 给与一个简单的告知 */
#warning "unknown platform memory check"
/* 某些环境下不需要刻意使用同步原语(比如单线程) */

void SGuiMemoryCheckReset(void)
{
}

void SGuiMemoryCheck(void)
{
}

void * SGuiMalloc(uint32_t size)
{
    SGuiMemoryCheckOptSync(true);
    void *pointer = SGUIPORTMALLOC(size);
    SGuiMemoryCheckOptSync(false);

    /* 可在此处添加分配失败断言 */
    return pointer;
}

void SGuiFree(void *pointer)
{
    SGuiMemoryCheckOptSync(true);
    SGUIPORTFREE(pointer);
    SGuiMemoryCheckOptSync(false);
}

#endif
