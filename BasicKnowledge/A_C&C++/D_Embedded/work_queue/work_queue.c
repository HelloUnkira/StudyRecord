//C std lib
#include <stddef.h>
#include <stdint.h>
//port lib
//内源依赖项
#include "sys_workset.h"

/*@prief 为工作队列就绪一份工作
 *@param[in]    workset 工作队列
 *@param[in]    work    一份工作
 */
void sys_workset_ready_work(sys_workset_t *workset, sys_work_t *work)
{
    work->next = workset->tail;
    
    if (workset->tail == NULL)
    {
        workset->head = work;
    }
    
    workset->tail = work;
}

/*@prief 为工作队列执行一份工作
 *@param[in]    workset 工作队列
 *@param[in]    work    一份工作
 *@retval   执行完毕的工作,其实体如何处理
 */
sys_work_t * sys_workset_exec_work(sys_workset_t *workset)
{
    /* 取出队首的工作 */
    sys_work_t *work = workset->head;
    
    if (work == NULL)
    {
        return NULL;
    }
    
    workset->head = work->next;
    
    if (workset->head == NULL)
    {
        workset->tail = NULL;
    }
    
    /* 执行队首的工作 */
    switch (work->type)
    {
	case 1:
		work->work1.work();
		break;	
    case 2:
        work->work2.work(work->work2.data);
        break;
    case 3:
        work->work3.work(work->work3.data, work->work3.size);
        break;
    }
    
    return work;
}

