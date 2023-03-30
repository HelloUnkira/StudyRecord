/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#if 0
#include <stdio.h>
#define ERROR_PRINT(Target, String) if (Target) printf("%s\n",(String));
#else
#define ERROR_PRINT(Target, String)
#endif
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 排序数据抽象 */
struct InternalSort_DataSet {
    /* 排序接口集合 */
    union {
        struct {
            /* 常规排序(元素大小比较,数据无关排序) */
            uint8_t  (*Compare)(void *Data1, void *Data2);
            /* 希尔排序扩充 */
            uint32_t (*Hill)(uint32_t Value);
        };
        struct {
            /* 位数据归类排序 */
            uint32_t (*BitsLength)(void);                       /* 基数排序(数据位总长) */
            bool     (*BitsLevel)(void *Data, uint32_t Vevel);  /* 基数排序(指定数据位) */
        };
    };
    /* 命中目标: */
    void     *List;    //通配元素集合
    uint32_t  Size;    //通配元素字节大小
    uint32_t  Left;    //排序起始点(相对于通配元素集合位置)
    uint32_t  Right;   //排序结束点(相对于通配元素集合位置)
};
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 核心转义 */
typedef struct InternalSort_DataSet InternalSort_Data;
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 常规排序(比较函数): */
//temp1严格小于temp2返回0
//temp1严格等于temp2返回1
//temp1严格大于temp2返回2
typedef uint8_t (*KeyCompare)(void *Temp1, void *Temp2);
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 常规排序(希尔排序扩充函数): */
//希尔增量序列,已知序列后一项求序列前一项,直到1
typedef uint32_t (*KeyHill)(uint32_t x);
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 位数据归类排序: */
//获得该类型的关键字位数
typedef uint32_t (*KeyBitsLength)(void);
//获得Temp的关键字的指定Pos位的状态
typedef bool (*KeyBitsLevel)(void *Temp, uint32_t Level);
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 结果状态记录 */
enum {LESS = 0, EQUAL = 1, GREATER = 2} IS_CompareState;
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 通配辅助函数:拷贝函数 */
static void InternalSort_Copy(uint8_t *Stream1, uint8_t *Stream2, uint32_t Length)
{
    ERROR_PRINT(Stream1 == NULL, "InternalSort_Copy: Stream1");
    ERROR_PRINT(Stream2 == NULL, "InternalSort_Copy: Stream2");
    /* 这里暂时没有想到很好的字节对齐拷贝的方式 */
    /* 因为两个流都有可能面临都不是地址字节对齐的情况 */
    /* 所以只能尝试从高到低进行拷贝 */
    for (0; Length >= sizeof(uint64_t); Length -= sizeof(uint64_t)) {
        *((uint64_t *)Stream1) = *((uint64_t *)Stream2);
        Stream1 += sizeof(uint64_t);
        Stream2 += sizeof(uint64_t);
    }
    for (0; Length >= sizeof(uint32_t); Length -= sizeof(uint32_t)) {
        *((uint32_t *)Stream1) = *((uint32_t *)Stream2);
        Stream1 += sizeof(uint32_t);
        Stream2 += sizeof(uint32_t);
    }
    for (0; Length >= sizeof(uint16_t); Length -= sizeof(uint16_t)) {
        *((uint16_t *)Stream1) = *((uint16_t *)Stream2);
        Stream1 += sizeof(uint16_t);
        Stream2 += sizeof(uint16_t);
    }
    for (0; Length >= sizeof(uint8_t);  Length -= sizeof(uint8_t)) {
        *((uint8_t  *)Stream1) = *((uint8_t  *)Stream2);
        Stream1 += sizeof(uint8_t);
        Stream2 += sizeof(uint8_t);
    }
    /*  */
    ERROR_PRINT(Length != 0, "InternalSort_Copy: Length");
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 通配辅助函数:交换函数 */
static void InternalSort_Swap(uint8_t *Stream1, uint8_t *Stream2, uint32_t Length)
{
    ERROR_PRINT(Stream1 == NULL, "InternalSort_Swap: Stream1");
    ERROR_PRINT(Stream2 == NULL, "InternalSort_Swap: Stream2");
    /*  */
    uint64_t Temp64_1 = 0, Temp64_2 = 0;
    uint32_t Temp32_1 = 0, Temp32_2 = 0;
    uint16_t Temp16_1 = 0, Temp16_2 = 0;
    uint8_t  Temp8_1  = 0, Temp8_2  = 0;
    /* 这里暂时没有想到很好的字节对齐拷贝的方式 */
    /* 因为两个流都有可能面临都不是地址字节对齐的情况 */
    /* 所以只能尝试从高到低进行拷贝 */
    for (0; Length >= sizeof(uint64_t); Length -= sizeof(uint64_t)) {
        Temp64_1 = (*((uint64_t *)Stream1));
        Temp64_2 = (*((uint64_t *)Stream2));
        *((uint64_t *)Stream1) = Temp64_2;
        *((uint64_t *)Stream2) = Temp64_1;
        Stream1 += sizeof(uint64_t);
        Stream2 += sizeof(uint64_t);
    }
    for (0; Length >= sizeof(uint32_t); Length -= sizeof(uint32_t)) {
        Temp32_1 = (*((uint32_t *)Stream1));
        Temp32_2 = (*((uint32_t *)Stream2));
        *((uint32_t *)Stream1) = Temp32_2;
        *((uint32_t *)Stream2) = Temp32_1;
        Stream1 += sizeof(uint32_t);
        Stream2 += sizeof(uint32_t);
    }
    for (0; Length >= sizeof(uint16_t); Length -= sizeof(uint16_t)) {
        Temp16_1 = (*((uint16_t *)Stream1));
        Temp16_2 = (*((uint16_t *)Stream2));
        *((uint16_t *)Stream1) = Temp16_2;
        *((uint16_t *)Stream2) = Temp16_1;
        Stream1 += sizeof(uint16_t);
        Stream2 += sizeof(uint16_t);
    }
    for (0; Length >= sizeof(uint8_t);  Length -= sizeof(uint8_t)) {
        Temp8_1 = (*((uint8_t *)Stream1));
        Temp8_2 = (*((uint8_t *)Stream2));
        *((uint8_t *)Stream1) = Temp8_2;
        *((uint8_t *)Stream2) = Temp8_1;
        Stream1 += sizeof(uint8_t);
        Stream2 += sizeof(uint8_t);
    }
    /*  */
    ERROR_PRINT(Length != 0, "InternalSort_Swap: Length");
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 宏定义简化及优化: */
static inline void InternalSort_Simplification(void)
{
/* 数据错误检查 */
#define INTERNALSORT_DATACHECK(DataSet)     \
    if ((DataSet)          == NULL ||       \
        (DataSet)->List    == NULL ||       \
        (DataSet)->Size    == 0    ||       \
        (DataSet)->Left    >=               \
        (DataSet)->Right)                   \
        return;                             \
    if ((DataSet)->Compare == NULL ||       \
       ((DataSet)->BitsLength == NULL &&    \
        (DataSet)->BitsLevel  == NULL))     \
        return;                             \

/* 数据所需参数定义 */
#define INTERNALSORT_DATADEFINE(DataSet)        \
    uint8_t *List       = (DataSet)->List;      \
    uint32_t Size       = (DataSet)->Size;      \
    uint32_t Left       = (DataSet)->Left;      \
    uint32_t Right      = (DataSet)->Right;     \
    KeyCompare    Compare    = (DataSet)->Compare;      \
    KeyHill       Hill       = (DataSet)->Hill;         \
    KeyBitsLength BitsLength = (DataSet)->BitsLength;   \
    KeyBitsLevel  BitsLevel  = (DataSet)->BitsLevel;    \

#define RETURN_EMPTY(target) if (target) return;

}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 冒泡排序O(n**2) */
void InternalSort_Bubble(void *DataSet)
{
    INTERNALSORT_DATACHECK((InternalSort_Data *)DataSet);
    INTERNALSORT_DATADEFINE((InternalSort_Data *)DataSet);
    
    uint32_t I = Left, J = Left;
    uint8_t *Index1 = NULL;
    uint8_t *Index2 = NULL;
    for (Index1 = List + Size * I; I <= Right; I++, Index1 += Size, J = I)
    for (Index2 = List + Size * J; J <= Right; J++, Index2 += Size)
        if (Compare(Index1, Index2) == GREATER)
            InternalSort_Swap(Index1, Index2, Size);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 选择排序O(n**2) */
void InternalSort_Selection(void *DataSet, void *Helper)
{
    INTERNALSORT_DATACHECK((InternalSort_Data *)DataSet);
    INTERNALSORT_DATADEFINE((InternalSort_Data *)DataSet);
    RETURN_EMPTY(Helper == NULL);
    
    uint32_t I = Left, J = Left;
    uint8_t *Index1 = NULL;
    uint8_t *Index2 = NULL;
    
    for (Index1 = List + Size * I; I <= Right; I++, J = I, Index1 += Size) {
        InternalSort_Copy(Helper, Index1, Size);
    for (Index2 = List + Size * J; J <= Right; J++, Index2 += Size)
        if (Compare(Helper, Index2) == GREATER)
            InternalSort_Swap(Helper, Index2, Size);
        InternalSort_Copy(Index1, Helper, Size);
    }
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 插入排序O(n**2) */
void InternalSort_Insertion(void *DataSet, void *Helper)
{
    INTERNALSORT_DATACHECK((InternalSort_Data *)DataSet);
    INTERNALSORT_DATADEFINE((InternalSort_Data *)DataSet);
    RETURN_EMPTY(Helper == NULL);
    
    uint32_t I = Left, J = Left;
    uint8_t *Index1 = NULL;
    uint8_t *Index2 = NULL;
    for (Index1 = List + Size * I; I <= Right; I++, J = I, Index1 += Size) {
        InternalSort_Copy(Helper, Index1, Size);
    for (Index2 = List + Size * J; J >= Left + 1; J--, Index2 -= Size) {
        if (Compare(Helper, (Index2 - Size)) != LESS)
            break;
        InternalSort_Copy(Index2, Index2 - Size, Size);
    }
        InternalSort_Copy(Index2, Helper, Size);
    }
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 希尔排序O(n log n) */
void InternalSort_Shell(void *DataSet, void *Helper)
{
    INTERNALSORT_DATACHECK((InternalSort_Data *)DataSet);
    INTERNALSORT_DATADEFINE((InternalSort_Data *)DataSet);
    RETURN_EMPTY(Helper == NULL || Hill == NULL);
    
    uint32_t K = Right - Left;
    do {/* 获取增量进行希尔间隔确认 */
        uint32_t Offset = Hill(K);
        K = (Offset > K) ? 1 : Offset;
        K = (0 == K) ? 1 : K;
        uint32_t Size_k = Size * K;
        
        for (uint32_t S = Left; S < Left + K; S++) 
        for (uint32_t I = S, J = I; I <= Right; I += K, J = I) {
             uint8_t *Index1 = List + Size * I;
             InternalSort_Copy(Helper, Index1, Size);
             uint8_t *Index2 = List + Size * J;
            
            for(0; I >= J && J >= Left + K; J -= K, Index2 -= Size_k) {
                if (Compare(Helper, (Index2 - Size_k)) != LESS) 
                    break;
                InternalSort_Copy(Index2, Index2 - Size_k, Size);
            }
            
            InternalSort_Copy(Index2, Helper, Size);
        }
    } while (K > 1);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 单次堆排序O(n log n) */
static void InternalSort_HeapOnly(uint8_t *List,  uint8_t  Size,
                                  uint32_t Scale, uint32_t Index,
                                  KeyCompare Compare)
{
    uint32_t Father     = Index;
    uint32_t LeftChild  = 0;
    uint32_t RightChild = 0;
    while (1) {
        LeftChild  = 2 * Father + 1;
        RightChild = 2 * Father + 2;
        
        /* 情况1:左右孩子都存在(依照特性如果右孩子存在左孩子一定存在) */
        if (RightChild <= Scale) {
            if (Compare((List + Size * Father),
                        (List + Size * LeftChild))  != LESS &&
                Compare((List + Size * Father),
                        (List + Size * RightChild)) != LESS)
            break;
            /* 如不是强迫症,谁又会这么写程序呢 */
            if (Compare((List + Size * LeftChild),
                        (List + Size * RightChild)) != LESS) {
                InternalSort_Swap(List + Size * Father,
                                  List + Size * LeftChild, Size);
                Father = LeftChild;
                continue;
            }
            if (Compare((List + Size * LeftChild),
                        (List + Size * RightChild)) != GREATER) {
                InternalSort_Swap(List + Size * Father,
                                  List + Size * RightChild, Size);
                Father = RightChild;
                continue;
            }
        }
        /* 情况2:仅左孩子存在(依照特性如果右孩子存在左孩子一定存在) */
        if (LeftChild  <= Scale) {
            if (Compare((List + Size * Father),
                        (List + Size * LeftChild)) != LESS)
                break;
            InternalSort_Swap(List + Size * Father,
                              List + Size * LeftChild, Size);
            Father = LeftChild;
            continue;
        }
        /* 情况3:左右孩子都不存在 */
        break;
    }
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 堆排序O(n log n) */
void InternalSort_Heap(void *DataSet)
{
    INTERNALSORT_DATACHECK((InternalSort_Data *)DataSet);
    INTERNALSORT_DATADEFINE((InternalSort_Data *)DataSet);

    /* 堆排序设计时,需要满足起始位置条件是i=0开始 */
    /* 才会满足左右孩子为2i+1,2i+2 */
    int I = 0, J = 0;
    uint32_t Number = Right - Left + 1;

    /* 计算最大非叶子节点落点 */
    for (I = 0; I * 2 + 2 < Number; I++);
    /* 先从下往上,将其调整为最大二叉堆 */
    while(I >= 0)
        InternalSort_HeapOnly(List, Size, Right, Left + I--, Compare);
    /* 将其进行堆排序调整 */
    for (I = 1, J = Number - 1; J > 0; J--, I++) {
        /* 交换堆头与堆尾,范围缩小一个 */
        InternalSort_Swap(List + Size * Left, List + Size * J, Size);
        /* 重新调整为堆,此时的堆调整从上至下调整 */
        InternalSort_HeapOnly(List, Size, Right - I, Left, Compare);
    }
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 单次归并排序O(n log n) */
static void InternalSort_MergeOnly(uint8_t *List, uint8_t *Temp,   uint8_t  Size,
                                   uint32_t Left, uint32_t Center, uint32_t Right,
                                   KeyCompare Compare)
{
    /* 检查边界值不失为一个好的习惯 */
    if (Left > Center || Center + 1 > Right)
        return;
    /* 至少有一个子序列是空的,无需合并 */
    uint32_t I = Left, J = Center + 1;
    uint32_t K = Left;
    uint8_t result = (~0);
    while (K <= Right) {
        
        /* 有一个子列已经为空 */
        if (I > Center) {
            InternalSort_Copy(Temp + Size * K++, List + Size * J++, Size);
            continue;
        }
        if (J > Right) {
            InternalSort_Copy(Temp + Size * K++, List + Size * I++, Size);
            continue;
        }
        /* 子列均不为空 */
        result = Compare((void *)(List + Size * I),
                         (void *)(List + Size * J));

        if (result == LESS) {
            InternalSort_Copy(Temp + Size * K++, List + Size * I++, Size);
            continue;
        }
        if (result != LESS) {
            InternalSort_Copy(Temp + Size * K++, List + Size * J++, Size);
            continue;
        }
    }

    InternalSort_Copy(List + Size * Left,
                      Temp + Size * Left, Size * (Right - Left + 1));
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 归并排序O(n log n) */
void InternalSort_Merge(void *DataSet, void *Helper)
{
    INTERNALSORT_DATACHECK((InternalSort_Data *)DataSet);
    INTERNALSORT_DATADEFINE((InternalSort_Data *)DataSet);
    RETURN_EMPTY(Helper == NULL);
    
    for (uint32_t H = 1; H < Right - Left + 1; H *= 2)
    {
        uint32_t LeftTemp   = Left;
        uint32_t CenterTemp = LeftTemp       + H - 1;
        uint32_t RightTemp  = CenterTemp + 1 + H - 1;//CenterTemp + H;
        /* 开始合并 */
        while (RightTemp < Right) {
            InternalSort_MergeOnly(List, Helper, Size,
                                   LeftTemp, CenterTemp, RightTemp, Compare);
            
            LeftTemp   = RightTemp  + 1;
            CenterTemp = LeftTemp   + H - 1;
            RightTemp  = CenterTemp + H;
        }
        
        if (LeftTemp < Right && CenterTemp < Right)
            InternalSort_MergeOnly(List, Helper, Size,
                                   LeftTemp, CenterTemp, Right, Compare);
    }
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 单次快速排序O(n log n) */
static void InternalSort_QuickOnly(uint8_t  *List,  uint8_t  Size,
                                   uint32_t  Left,  uint32_t Right,
                                   uint32_t *Index, KeyCompare Compare)
{
    if (Left > Right || Index == NULL)
        return;
    if (Left == Right) {
        *Index = Left;
        *Index = Right;
        return;
    }
    /* 选取基准值采用三分中值的方式(经典) */
    uint32_t Center   = Left + (Right - Left) / 2;
    uint32_t Compare1 = Compare((List + Size * Center), (List + Size * Left));
    uint32_t Compare2 = Compare((List + Size * Center), (List + Size * Right));
    uint32_t Compare3 = Compare((List + Size * Left),   (List + Size * Right));
    uint32_t BorderLeft = Left;
    uint32_t BorderRight = Right;
    /*  Center <= Left  ---> Compare1 != GREATER    */
    /*  Center >= Left  ---> Compare1 != LESS       */
    /*  Center <= Right ---> Compare2 != GREATER    */
    /*  Center >= Right ---> Compare2 != LESS       */
    /*    Left <= Right ---> Compare3 != GREATER    */
    /*    Left >= Right ---> Compare3 != LESS       */
    
    /* Left <= Center <= Right || Right <= Center <= Left   */
    /* if (((Compare1 != LESS) && (Compare2 != GREATER)) || */
    /*     ((Compare2 != LESS) && (Compare1 != GREATER)))   */
    /*     Center = Center;                                 */
    
    /* Center <= Left <= Right || Right <= Left <= Center   */
    if (((Compare1 != GREATER) && (Compare3 != GREATER)) ||
        ((Compare3 != LESS)    && (Compare1 != LESS)))
        Center = Left;
    /* Left <= Right <= Center || Center <= Right <= Left   */
    if (((Compare3 != GREATER && Compare2 != LESS)) ||
        ((Compare2 != GREATER && Compare3 != LESS)))
        Center = Right;

    /* 参考基准值进行比较交换 */
    while (true) {
        /* 寻找第一个 Left > Center,则当 Left <= Center 时 */
        while (Left  < BorderRight &&
               Compare((List + Size * Left),
                       (List + Size * Center)) != GREATER)
            Left++;
        /* 寻找第一个 Right < Center,则当 Right >= Center 时 */
        while (Right > BorderLeft  &&
               Compare((List + Size * Right),
                       (List + Size * Center)) != LESS)
            Right--;
        /* 如果没找到时,准备退出 */
        if (Left >= Right) {
            if (Right >= Center) {
                InternalSort_Swap(List + Size * Center,
                                  List + Size * Right, Size);
                Center = Right;
            }
            if (Left  <= Center) {
                InternalSort_Swap(List + Size * Center,
                                  List + Size * Left,  Size);
                Center = Left;
            }
            break;
        }
        /* 交换 */
        if (Compare((List + Size * Left), (List + Size * Center)) != EQUAL && 
            Compare((List + Size * Right),(List + Size * Center)) != EQUAL)
            InternalSort_Swap(List + Size * Left, List + Size * Right, Size);
    }
    *Index = Center;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 快速排序O(n log n) */
void InternalSort_Quick(void *DataSet, void *Helper)
{
    INTERNALSORT_DATACHECK((InternalSort_Data *)DataSet);
    INTERNALSORT_DATADEFINE((InternalSort_Data *)DataSet);
    RETURN_EMPTY(Helper == NULL);

    int32_t Number = 0;
    uint32_t *Temp = (uint32_t *)Helper;

    /* 首项入栈: */
    Temp[++Number] = Left;
    Temp[++Number] = Right;
    
    uint32_t BorderLeft  = Left;
    uint32_t BorderRight = Right;
    /* 开始栈追踪 */
    do  {
        /* 出栈: */
        uint32_t Center = 0;
        Right = Temp[Number--];
        Left  = Temp[Number--];
        
        InternalSort_QuickOnly(List, Size, Left, Right, &Center, Compare);
        
        /* 右子项入栈,左子项入栈,注意判断以防止沉入死循环 */
        if (Center + 1 < Right) {
            Temp[++Number] = Center + 1;
            Temp[++Number] = Right;
        }
        
        /* 又菜又爱玩:(uint32_t)(0) - 1 == (uint32_t)(~0) */
        if (Center == 0)
            continue;
        
        if (Center - 1 > Left) {
            Temp[++Number] = Left;
            Temp[++Number] = Center - 1;
        }
    } while (Number != 0);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 基数排序O(n log n) */
static void InternalSort_RadixOnly(uint8_t  *List,    uint8_t   Size,
                                   uint32_t  Left,    uint32_t  Right,
                                   uint32_t *LeftNew, uint32_t *RightNew,
                                   uint32_t  Level,   uint32_t  BitsLength,
                                   KeyBitsLevel BitsLevel)
{
    if (Level == 0)
        return;
    if (Left == Right) {
        *LeftNew  = Left;
        *RightNew = Right;
        return;
    }
    /* 这里使用Level - 1进行单次迭代 */
    uint32_t BorderLeft = Left;
    uint32_t BorderRight = Right;

    while (true) {
        uint8_t *Index1 = NULL;
        uint8_t *Index2 = NULL;
        /* 在左边找到第一个逆项 */
        while (Left  < BorderRight &&
              !BitsLevel(List + Size * Left,  Level - 1))
               Left++;               
        /* 在右边找到第一个逆项 */
        while (Right > BorderLeft  &&
               BitsLevel(List + Size * Right, Level - 1))
               Right--;
        /* 如果没找到时,准备退出 */
        if (Left >= Right) {
           *LeftNew  = Right;
           *RightNew = Left;
            break;
        }
        /* 交换 */
        InternalSort_Swap(List + Size * Left, List + Size * Right, Size);
    }
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 基数排序O(n log n) */
void InternalSort_Radix(void *DataSet, void *Helper)
{
    INTERNALSORT_DATACHECK((InternalSort_Data *)DataSet);
    INTERNALSORT_DATADEFINE((InternalSort_Data *)DataSet);
    RETURN_EMPTY(Helper == NULL);
    
    uint32_t Length = BitsLength();

    int32_t Number = 0;
    uint32_t *Temp = (uint32_t *)Helper;

    /* 首项入栈: */
    Temp[++Number] = Length;
    Temp[++Number] = Left;
    Temp[++Number] = Right;
    
    uint32_t BorderLeft  = Left;
    uint32_t BorderRight = Right;
    /* 开始栈追踪 */
    do  {
        /* 出栈: */
        uint32_t Center = 0;
        Right = Temp[Number--];
        Left  = Temp[Number--];
        uint32_t Level = Temp[Number--];
        uint32_t LeftNew  = 0;
        uint32_t RightNew = 0;
        
        InternalSort_RadixOnly(List, Size, Left, Right, &LeftNew, &RightNew,
                               Level, Length, BitsLevel);
        if (Level == 0)
            continue;
        /* 右子项入栈,左子项入栈 */
        if (Left < LeftNew) {
            Temp[++Number] = Level - 1;
            Temp[++Number] = Left;
            Temp[++Number] = LeftNew;
        }
        if (RightNew < Right) {
            Temp[++Number] = Level - 1;
            Temp[++Number] = RightNew;
            Temp[++Number] = Right;
        }
    } while (Number != 0);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 奇偶排序O(n**2) */
void InternalSort_OddEven(void *DataSet)
{
    INTERNALSORT_DATACHECK((InternalSort_Data *)DataSet);
    INTERNALSORT_DATADEFINE((InternalSort_Data *)DataSet);
    
    bool Loop = true;
    uint32_t I = Left;
    uint8_t *Index1 = NULL;
    uint8_t *Index2 = NULL;
    
    do {
        Loop = false;
        /* 奇排序 */
        for (I = Left,
             Index1 = List + Size * (I),
             Index2 = List + Size * (I + 1);
             I + 1 <= Right; I += 2, Index1 += Size * 2, Index2 += Size * 2)
             if (Compare(Index1, Index2) == GREATER) {
                 InternalSort_Swap(Index1, Index2, Size);
                 Loop = true;
             }
        /* 偶排序 */
        for (I = Left,
             Index1 = List + Size * (I + 1),
             Index2 = List + Size * (I + 2);
             I + 2 <= Right; I += 2, Index1 += Size * 2, Index2 += Size * 2)
             if (Compare(Index1, Index2) == GREATER) {
                 InternalSort_Swap(Index1, Index2, Size);
                 Loop = true;
             }
    } while (Loop);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 单次双调合并排序O(n log n) */
static void InternalSort_BitonicMerge(uint8_t *List, uint8_t  Size,
                                      uint32_t Left, uint32_t Right, bool Direct,
                                      KeyCompare Compare)
{
    if (Left >= Right)
        return;
    /* 切分点 */
    uint32_t Number = Right - Left + 1;
    uint32_t Middle = 1;
    while (Middle < Number)
           Middle <<= 1;
           Middle >>= 1;
    /* 迭代 */
    for (uint32_t Idx = 0; Idx < Number - Middle; Idx++) {
        uint8_t *Index1 = List + Size * (Idx + Left);
        uint8_t *Index2 = List + Size * (Idx + Left + Middle);
        if ((Compare(Index1, Index2) == GREATER &&  Direct) ||
            (Compare(Index1, Index2) != GREATER && !Direct))
             InternalSort_Swap(Index1, Index2, Size);
    }
    InternalSort_BitonicMerge(List, Size, Left,  Left + Middle - 1, Direct, Compare);
    InternalSort_BitonicMerge(List, Size, Left + Middle, Right,     Direct, Compare);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 单次双调排序O(n log n) */
static void InternalSort_BitonicOnly(uint8_t *List, uint8_t  Size,
                                     uint32_t Left, uint32_t Right, bool Direct,
                                     KeyCompare Compare)
{
    if (Left >= Right)
        return;
    /* 切分点 */
    uint32_t Number = Right - Left + 1;
    uint32_t Middle = Number / 2;
    InternalSort_BitonicOnly(List,  Size, Left,  Left + Middle - 1, !Direct, Compare);
    InternalSort_BitonicOnly(List,  Size, Left + Middle, Right,      Direct, Compare);
    InternalSort_BitonicMerge(List, Size, Left,  Right,              Direct, Compare);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 双调排序O(n log n) */
void InternalSort_Bitonic(void *DataSet)
{
    INTERNALSORT_DATACHECK((InternalSort_Data *)DataSet);
    INTERNALSORT_DATADEFINE((InternalSort_Data *)DataSet);
    
    InternalSort_BitonicOnly(List, Size, Left, Right, true, Compare);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
///////////////////////////////////////////////////////////////////////////////
//计数排序:(特定使用场景,不通用,实现简单)//////////////////////////////////////
//桶排序:(链表集,空间开销更大)/////////////////////////////////////////////////
//基数排序:(位优先排序,效果不错,需要链表或同比例外部)//////////////////////////
///////////////////////////////////////////////////////////////////////////////
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void InternalSort_SetDataSet(void *DataSet, void *List, uint32_t Size, uint32_t Left, uint32_t Right)
{
    ((InternalSort_Data *)DataSet)->List  = List;
    ((InternalSort_Data *)DataSet)->Size  = Size;
    ((InternalSort_Data *)DataSet)->Left  = Left;
    ((InternalSort_Data *)DataSet)->Right = Right;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void InternalSort_SetCompare(void *DataSet, KeyCompare Compare)
{
    ((InternalSort_Data *)DataSet)->Compare = Compare;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void InternalSort_SetHill(void *DataSet, KeyHill Hill)
{
    ((InternalSort_Data *)DataSet)->Hill = Hill;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void InternalSort_SetBitsLength(void *DataSet, KeyBitsLength BitsLength)
{
    ((InternalSort_Data *)DataSet)->BitsLength = BitsLength;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void InternalSort_SetBitsLevel(void *DataSet, KeyBitsLevel BitsLevel)
{
    ((InternalSort_Data *)DataSet)->BitsLevel = BitsLevel;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
