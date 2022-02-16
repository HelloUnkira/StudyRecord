#define SIMPLE_SORT_CORE
#ifdef  SIMPLE_SORT_CORE
///////////////////////////////////////////////////////////////////////////////
//排序容器核心/////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
typedef unsigned char (*key_compare)(void *temp1, void *temp2);

typedef struct sort_override_data {
    key_compare  compare; //通配元素间大小关系比较函数
    void        *list;    //通配元素集合
    unsigned int size;    //通配元素字节大小
    unsigned int left;    //排序起始点(相对于通配元素集合位置)
    unsigned int right;   //排序结束点(相对于通配元素集合位置)
} sort_data_t;

enum {
    LESS    = 0,
    EQUAL   = 1,
    GREATER = 2
};
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
#endif

#define SIMPLE_SORT_MACRO_SIMPLIFICATION
#ifdef  SIMPLE_SORT_MACRO_SIMPLIFICATION

#define NULL (void *)0

#define SORT_DATA_CHECK(basic)      \
    if ((basic)          == NULL || \
        (basic)->compare == NULL || \
        (basic)->list    == NULL || \
        (basic)->size    == 0    || \
        (basic)->left    >=         \
        (basic)->right)             \
        return;

#define SORT_DATA_DEFINE(basic)             \
    key_compare compare = (basic)->compare; \
    unsigned char *list = (basic)->list;    \
    unsigned int size   = (basic)->size;    \
    unsigned int left   = (basic)->left;    \
    unsigned int right  = (basic)->right;

#define IF_RETURN(condition, work) \
    if (condition)                 \
        work;                      \

#endif

#define SIMPLE_SORT_AUXILIARY_MACRO
#ifdef  SIMPLE_SORT_AUXILIARY_MACRO

///////////////////////////////////////////////////////////////////////////////
//交换函数:////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
static void swap(unsigned char *str1, unsigned char *str2, unsigned int size)
{
    IF_RETURN(str1 == NULL || str2 == NULL, return);
    //充满着美好的幻想,同样的,强迫症不是一朝一夕形成的
#define SIMPLE_SWAP_SIMPLIFICATION(flag) \
    while (size >= sizeof(flag)) {       \
        flag temp_1 = *((flag *)str1);   \
        flag temp_2 = *((flag *)str2);   \
        *((flag *)str1) = temp_2;        \
        *((flag *)str2) = temp_1;        \
        str1 += sizeof(flag);            \
        str2 += sizeof(flag);            \
        size -= sizeof(flag);            \
    }
    
    SIMPLE_SWAP_SIMPLIFICATION(unsigned long long)
    SIMPLE_SWAP_SIMPLIFICATION(unsigned int)
    SIMPLE_SWAP_SIMPLIFICATION(unsigned short)
    SIMPLE_SWAP_SIMPLIFICATION(unsigned char)
}

///////////////////////////////////////////////////////////////////////////////
//拷贝函数:////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
static void copy(unsigned char *str1, unsigned char *str2, unsigned int size)
{
    IF_RETURN(str1 == NULL || str2 == NULL, return);
    //充满着美好的幻想,同样的,强迫症不是一朝一夕形成的
#define SIMPLE_COPY_SIMPLIFICATION(flag)   \
    while (size >= sizeof(flag)) {         \
        *((flag *)str1) = *((flag *)str2); \
        str1 += sizeof(flag);              \
        str2 += sizeof(flag);              \
        size -= sizeof(flag);              \
    }
    
    SIMPLE_COPY_SIMPLIFICATION(unsigned long long)
    SIMPLE_COPY_SIMPLIFICATION(unsigned int)
    SIMPLE_COPY_SIMPLIFICATION(unsigned short)
    SIMPLE_COPY_SIMPLIFICATION(unsigned char)
}

#endif

#define SIMPLE_SORT_ENTITY
#ifdef  SIMPLE_SORT_ENTITY

///////////////////////////////////////////////////////////////////////////////
//冒泡排序O(n**2)//////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void bubble_sort(void *basic)
{
    SORT_DATA_CHECK((sort_data_t *)basic);
    SORT_DATA_DEFINE((sort_data_t *)basic);
    
    unsigned int i = left, j = left;
    unsigned char *index1 = NULL;
    unsigned char *index2 = NULL;
    for (index1 = list + size * i; i <= right; i++, j = i, index1 += size)
        for (index2 = list + size * j; j <= right; j++, index2 += size)
            if (compare((void *)index1, (void *)index2) == GREATER)
                swap(index1, index2, size);
}

///////////////////////////////////////////////////////////////////////////////
//选择排序O(n**2)//////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void selection_sort(void *basic, void *helper)
{
    SORT_DATA_CHECK((sort_data_t *)basic);
    SORT_DATA_DEFINE((sort_data_t *)basic);
    IF_RETURN(helper == NULL, return);
    
    unsigned int i = left, j = left;
    unsigned char *index1 = NULL;
    unsigned char *index2 = NULL;
    unsigned char *temp   = (unsigned char *)helper;
    
    for (index1 = list + size * i; i <= right; i++, j = i, index1 += size) {
        copy(temp, index1, size);
        for (index2 = list + size * j; j <= right; j++, index2 += size)
            if (compare(temp, index2) == GREATER)
                swap(temp, index2, size);
        copy(index1, temp, size);
    }
}

///////////////////////////////////////////////////////////////////////////////
//插入排序O(n**2)//////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void insertion_sort(void *basic, void *helper)
{
    SORT_DATA_CHECK((sort_data_t *)basic);
    SORT_DATA_DEFINE((sort_data_t *)basic);
    IF_RETURN(helper == NULL, return);
    
    unsigned int i = left, j = left;
    unsigned char *index1 = NULL;
    unsigned char *index2 = NULL;
    unsigned char *temp   = (unsigned char *)helper;
    for (index1 = list + size * i; i <= right; i++, j = i, index1 += size) {
        copy(temp, index1, size);
        for (index2 = list + size * j; j >= left + 1; j--, index2 -= size) {
            if (compare((void *)temp, (void *)(index2 - size)) != LESS)
                break;
            copy(index2, index2 - size, size);
        }
        copy(index2, temp, size);
    }
}

///////////////////////////////////////////////////////////////////////////////
//希尔排序O(n log n)///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
typedef unsigned int (*hill_sequence)(unsigned int x);
void shell_sort(void *basic, void *helper, hill_sequence incremental)
{
    SORT_DATA_CHECK((sort_data_t *)basic);
    SORT_DATA_DEFINE((sort_data_t *)basic);
    IF_RETURN(helper == NULL || incremental == NULL, return);
    
    unsigned int s = left, i = left, j = left;
    unsigned int k = right - left, offset = 0, size_k = 0;
    unsigned char *index1 = NULL;
    unsigned char *index2 = NULL;
    unsigned char *temp   = (unsigned char *)helper;
    do {//获取增量进行希尔间隔确认
        offset = incremental(k);
        k = (offset > k) ? 1 : offset;
        k = (0 == k) ? 1 : k;
        size_k = size * k;
        
        for (s = left; s < left + k; s++) 
            for (i = s, j = i; i <= right; i += k, j = i) {
                index1 = list + size * i;
                copy(temp, index1, size);
                
                for(index2 = list + size * j;
                    i >= j && j >= left + k;
                    j -= k, index2 -= size_k) {//想写作一行的
                        
                    if (compare((void *)temp,
                                (void *)(index2 - size_k)) != LESS) 
                        break;
                    copy(index2, index2 - size_k, size);
                }
                
                copy(index2, temp, size);
            }
    } while (k > 1);
}

///////////////////////////////////////////////////////////////////////////////
//堆排序O(n log n)/////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
static void simple_heap(unsigned char *list,
                        unsigned char  size,
                        key_compare compare,
                        unsigned int  scale,
                        unsigned int  index)
{
    unsigned int father      = index;
    unsigned int left_child  = 0;
    unsigned int right_child = 0;
    while (1) {
        left_child  = 2 * father + 1;
        right_child = 2 * father + 2;
        
        //情况1:左右孩子都存在(依照特性如果右孩子存在左孩子一定存在)
        if (right_child <= scale) {
            if (compare((void *)(list + size * father),
                        (void *)(list + size * left_child))  != LESS &&
                compare((void *)(list + size * father),
                        (void *)(list + size * right_child)) != LESS)
            break;
            //如不是强迫症,谁又会这么写程序呢
            if (compare((void *)(list + size * left_child),
                        (void *)(list + size * right_child)) != LESS) {
                swap(list + size * father, list + size * left_child, size);
                father = left_child;
                continue;
            }
            if (compare((void *)(list + size * left_child),
                        (void *)(list + size * right_child)) != GREATER) {
                swap(list + size * father, list + size * right_child, size);
                father = right_child;
                continue;
            }
        }
        //情况2:仅左孩子存在(依照特性如果右孩子存在左孩子一定存在)
        if (left_child  <= scale) {
            if (compare((void *)(list + size * father),
                        (void *)(list + size * left_child)) != LESS)
                break;
            swap(list + size * father, list + size * left_child, size);
            father = left_child;
            continue;
        }
        //情况3:左右孩子都不存在
        break;
    }
}

void heap_sort(void *basic)
{
    SORT_DATA_CHECK((sort_data_t *)basic);
    SORT_DATA_DEFINE((sort_data_t *)basic);

    //堆排序设计时,需要满足起始位置条件是i=0开始
    //才会满足左右孩子为2i+1,2i+2
    int i = 0, j = 0;
    unsigned int number = right - left + 1;

    //计算最大非叶子节点落点
    for (i = 0; i * 2 + 2 < number; i++);
    //先从下往上,将其调整为最大二叉堆
    while(i >= 0)
        simple_heap(list, size, compare, right, left + i--);
    //将其进行堆排序调整
    for (i = 1, j = number - 1; j > 0; j--, i++) {
        //交换堆头与堆尾,范围缩小一个
        swap(list + size * left, list + size * j, size);
        //重新调整为堆,此时的堆调整从上至下调整
        simple_heap(list, size, compare, right - i, left);
    }
}


///////////////////////////////////////////////////////////////////////////////
//归并排序O(n log n)///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
static void simple_merge(unsigned char *list,
                         unsigned char *temp,
                         unsigned char  size,
                         key_compare compare,
                         unsigned int left,
                         unsigned int center,
                         unsigned int right)
{
    //检查边界值不失为一个好的习惯
    if (left > center || center + 1 > right)
        return;//至少有一个子序列是空的,无需合并
    unsigned int i = left, j = center + 1;
    unsigned int k = left;
    unsigned char result = (~0);
    while (k <= right) {

        //有一个子列已经为空
        if (i > center) {
            copy(temp + size * k++, list + size * j++, size);
            continue;
        }
        if (j > right) {
            copy(temp + size * k++, list + size * i++, size);
            continue;
        }
        //子列均不为空
        result = compare((void *)(list + size * i),
                         (void *)(list + size * j));

        if (result == LESS) {
            copy(temp + size * k++, list + size * i++, size);
            continue;
        }
        if (result != LESS) {
            copy(temp + size * k++, list + size * j++, size);
            continue;
        }
    }

    copy(list + size * left, temp + size * left, size * (right - left + 1));
}

void merge_sort(void *basic, void *helper)
{
    SORT_DATA_CHECK((sort_data_t *)basic);
    SORT_DATA_DEFINE((sort_data_t *)basic);
    IF_RETURN(helper == NULL, return);
    
    unsigned char *temp = (unsigned char *)helper;
    unsigned int temp_left   = 0;
    unsigned int temp_right  = 0;
    unsigned int temp_center = 0;
    unsigned int h = 1;
    
    for (h = 1; h < right - left + 1; h *= 2)
    {
        temp_left   = left;
        temp_center = temp_left       + h - 1;
        temp_right  = temp_center + 1 + h - 1;//temp_center + h;
        //开始合并
        while (temp_right < right) {
            simple_merge(list, temp, size, compare,
                temp_left, temp_center, temp_right);
            
            temp_left   = temp_right + 1;
            temp_center = temp_left + h - 1;
            temp_right  = temp_center + h;
        }
        
        if (temp_left < right && temp_center < right)
            simple_merge(list, temp, size, compare,
                temp_left, temp_center, right);
    }
}

///////////////////////////////////////////////////////////////////////////////
//快速排序O(n log n)///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
static void simple_quick(unsigned char *list,
                         unsigned char size,
                         key_compare compare,
                         unsigned int  left,
                         unsigned int  right,
                         unsigned int *index)
{
    if (left > right || index == NULL)
        return;
    if (left == right) {
        *index = left;
        *index = right;
        return;
    }
    //选取基准值采用三分中值的方式(经典)
    unsigned int center   = left + (right - left) / 2;
    unsigned int compare1 = compare((void *)(list + size * center),
                                    (void *)(list + size * left));
    unsigned int compare2 = compare((void *)(list + size * center),
                                    (void *)(list + size * right));
    unsigned int compare3 = compare((void *)(list + size * left),
                                    (void *)(list + size * right));
    unsigned int temp_left  = left;
    unsigned int temp_right = right;
    //  center <= left  ---> compare1 != GREATER
    //  center >= left  ---> compare1 != LESS
    //  center <= right ---> compare2 != GREATER
    //  center >= right ---> compare2 != LESS
    //    left <= right ---> compare3 != GREATER
    //    left >= right ---> compare3 != LESS
    
    //left <= center <= right || right <= center <= left
    //if (((compare1 != LESS) && (compare2 != GREATER)) ||
    //    ((compare2 != LESS) && (compare1 != GREATER)))
    //    center = center;
    //center <= left <= right || right <= left <= center
    if (((compare1 != GREATER) && (compare3 != GREATER)) ||
        ((compare3 != LESS)    && (compare1 != LESS)))
        center = left;
    //left <= right <= center || center <= right <= left
    if (((compare3 != GREATER && compare2 != LESS)) ||
        ((compare2 != GREATER && compare3 != LESS)))
        center = right;

    //参考基准值进行比较交换
    while (1) {
        //寻找第一个 left > center,则当 left <= center 时
        while (left  < temp_right &&
               compare((void *)(list + size * left),
                       (void *)(list + size * center)) != GREATER)
            left++;
        //寻找第一个 right < center,则当 right >= center 时
        while (right > temp_left  &&
               compare((void *)(list + size * right),
                       (void *)(list + size * center)) != LESS)
            right--;
        //如果没找到时,准备退出
        if (left >= right) {
            if (right >= center) {
                swap(list + size * center, list + size * right, size);
                center = right;
            }
            if (left  <= center) {
                swap(list + size * center, list + size * left, size);
                center = left;
            }
            break;
        }
        //交换
        if (compare((void *)(list + size * left),
                    (void *)(list + size * center)) != EQUAL && 
            compare((void *)(list + size * right),
                    (void *)(list + size * center)) != EQUAL)
            swap(list + size * left, list + size * right, size);
    }
    *index = center;
}

void quick_sort(void *basic, void *helper)
{
    SORT_DATA_CHECK((sort_data_t *)basic);
    SORT_DATA_DEFINE((sort_data_t *)basic);
    IF_RETURN(helper == NULL, return);

    unsigned int *temp = (unsigned int *)helper;
    unsigned int temp_left   = left;
    unsigned int temp_right  = right;
    unsigned int temp_center = 0;
    int number = 0;
    
    //首项入栈:
    temp[++number] = left;
    temp[++number] = right;
    
    //开始栈追踪
    do  {
        //出栈:
        temp_right = temp[number--];
        temp_left  = temp[number--];
        
        simple_quick(list, size, compare,
            temp_left, temp_right, &temp_center);
        
        //右子项入栈,左子项入栈,注意判断以防止沉入死循环
        if (temp_center + 1 < temp_right) {
            temp[++number] = temp_center + 1;
            temp[++number] = temp_right;
        }
        
        //又菜又爱玩:(unsigned int)(0) - 1 == (unsigned int)(~0)
        if (temp_center == 0)
            continue;
        
        if (temp_center - 1 > temp_left) {
            temp[++number] = temp_left;
            temp[++number] = temp_center - 1;
        }
    } while (number != 0);
}

///////////////////////////////////////////////////////////////////////////////
//计数排序:(特定使用场景,不通用,实现简单)//////////////////////////////////////
//桶排序:(链表集,空间开销更大)/////////////////////////////////////////////////
//基数排序:(位优先排序,效果不错,需要链表或同比例外部)//////////////////////////
///////////////////////////////////////////////////////////////////////////////

#endif

#define SIMPLE_SORT_OTHER
#ifdef  SIMPLE_SORT_OTHER

unsigned char get_sort_container_byte(void)
{
    return sizeof(sort_data_t);
}

void set_sort_container(void        *basic,
                        key_compare  compare,
                        void        *list,
                        unsigned int size,
                        unsigned int left,
                        unsigned int right)
{
    ((sort_data_t *)basic)->compare = compare;
    ((sort_data_t *)basic)->list    = list;
    ((sort_data_t *)basic)->size    = size;
    ((sort_data_t *)basic)->left    = left;
    ((sort_data_t *)basic)->right   = right;
}

#endif
