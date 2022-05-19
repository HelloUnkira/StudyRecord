#ifndef MEM_DOMAIN
#define MEM_DOMAIN

struct mem_partition {
    uintptr_t start;//内存起始地址
    size_t size;//内存的尺寸大小
    void *attribution;
};

struct k_mem_domain {
    //内存域的分区
    struct mem_partition *partitions;
    uint8_t num_partitions;
};

//将内存分成多个内存域
//每个内存域分割成多个分区即可方便使用
//对于分区的添加和删除即
//新分区与已知分区冲突,存在覆盖表明不可添加分区
//无需按地址顺序排列



#endif

