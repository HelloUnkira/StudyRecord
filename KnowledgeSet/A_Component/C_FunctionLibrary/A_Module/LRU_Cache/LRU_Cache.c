#define <stdint.h>
#define <stdbool.h>
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#define "DoubleList.h"
#define "LRU_Cache.h"
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
typedef struct LeastRecentlyUsed_Cache {
    uint8_t *EAddr;  /* 外存地址(External Address) */
    uint8_t *IAddr;  /* 内存地址(Internal Address) */
    uint32_t Length; /* 该数据块大小 */
    DL_Node  Node;
} LRU_Cache;
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
int32_t LRU_Cache_Take(LRU_Control *Control, LRU_Status *Status,
                       uint8_t *EAddr, uint8_t **IAddr, uint32_t Length)
{
    if (Control->Unit < Length) {
        /* 为资源申请所需的空间 */
        *Status = LRU_Status_MemFail;
        if ((*IAddr = Control->Alloc(Length)) == NULL)
            return 0;
        /* 为溢出资源提供标记信息 */
        *Status = LRU_Status_Overflow;
        return Control->Read(EAddr, *IAddr, Length);
    }
    if (Control->Unit >= Length) {
        LRU_Cache *Cache = NULL;
        /* 迭代缓存链表,尝试命中目标缓存 */
        /* 对称语义,二选其一 */
        //DL_List_Traverse_Backward(&(Control->List), Current)
        DL_List_Traverse_Forward(&(Control->List), Current) {
            LRU_Cache *Target = DL_GetOwner(LRU_Cache, Node, Current);
            /* 如果匹配到对应的区间(逻辑地址和长度均要一致) */
            if (Target->EAddr == EAddr && Target->Length == Length) {
                /* 如果命中缓存时 */
                *Status = LRU_Status_Hit;
                *IAddr  = Cache->IAddr;
               /* 命中的缓存移出并释放 */
               DL_List_Remove(&(Control->List), &(Cache->Node));
               Control->Usage -= Length;
               Control->Free(Cache);
               return 0;
            }
        }
        /* 为资源申请所需的空间 */
        *Status = LRU_Status_MemFail;
        if ((*IAddr = Control->Alloc(Length)) == NULL)
            return 0;
        *Status = LRU_Status_UnHit;
        return Control->Read(EAddr, *IAddr, Length);
    }
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
int32_t LRU_Cache_Give(LRU_Control *Control, LRU_Status *Status,
                       uint8_t *EAddr, uint8_t  *IAddr, uint32_t Length, bool Dirty)
{
    if (Control->Unit < Length) {
        /* 为溢出资源提供标记信息 */
        *Status = LRU_Status_Overflow;
        /* 溢出资源不接受缓存管理 */
        Control->Free(IAddr);
        return Control->Read(EAddr, IAddr, Length);
    }
    if (Control->Unit >= Length) {
        /* 申请一块缓存 */
        LRU_Cache *Cache = Control->Alloc(sizeof(LRU_Cache));
        *Status = LRU_Status_MemFail;
        if (Cache == NULL) {
            Control->Free(IAddr);
            return 0;
        }
        /* 配置该缓存属性 */
        Cache->EAddr = EAddr;
        Cache->IAddr = IAddr;
        Cache->Length = Length;
        DL_Node_Reset(&(Cache->Node));
        DL_List_InsertPrepend(&(Control->List), NULL, &(Cache->Node));
        Control->Usage += Length;
        /* 检查缓存总门限溢出情况 */
        while (Control->Usage > Control->Sum) {
            DL_Node *Current = DL_List_GetTail(&(Control->List));
            DL_List_Remove(&(Control->List), Current);
            LRU_Cache *Cache = DL_GetOwner(LRU_Cache, Node, Current);
            Control->Usage -= Cache->Length;
            Control->Free(Cache->IAddr);
            Control->Free(Cache);
        }
    }
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
