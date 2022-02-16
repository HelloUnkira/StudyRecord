
//内源依赖
#include "sys_workset.h"

//一个使用模板:

void sys_exec(sys_workset_t *workset)
{
    /*  工作队列的工作是外界派发
        所以它需要有对其回收或处理的方式 */
    sys_work_t *work = NULL;

    /* 已经完成的工作要及时销毁数据实体 */
    /* 这里是让上层确定销毁规则 */
    /* 这很重要,意味着所有为sys_exec提供服务的模组 */
    /* 都需要有一个指定的销毁规则,如下 */

    while ((work = sys_workset_exec_work(workset)) != NULL)
    {
        if (work->static_data)
        {
            switch (work->type)
            {
            case 2:
                SYS_FREE(work->work2.data);
                break;
            case 3:
                SYS_FREE(work->work3.data);
                break;
            }
        }
        
        if (work->static_body)
        {
            SYS_FREE(work);
        }
        
        work = NULL;
    }
}
