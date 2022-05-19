#ifndef MEM_SLAB
#define MEM_SLAB

struct mem_slab {
	uint32_t num_blocks;//SLAB内存区的块数
	size_t block_size;//单个SLAB内存块的大小
	char *buffer;//内存区的实际空间
	char *free_list;//空闲链表
	uint32_t num_used;//当前使用的块数
	uint32_t max_used;//历史最大使用的块数
};

//创建空闲链表
static int create_free_list(struct mem_slab *slab)
{
	char *p;

	//块大小和缓冲区大小需要字节对齐
	if (((slab->block_size | (uintptr_t)slab->buffer) &
		 (sizeof(void *) - 1)) != 0U) {
		return -1;
	}

	slab->free_list = NULL;
	p = slab->buffer;

	//初始化为一个逆向单链表
	// NULL <- 块0 <- 块1 <- 块2 <- …… <- 块n-1 <- 块n
	for (int j = 0U; j < slab->num_blocks; j++) {
		*(char **)p = slab->free_list;
		slab->free_list = p;
		p += slab->block_size;
	}
	//循环后,每一个项保存下一个项首地址
	//循环后,分配器的free_list指向最后一个块
	return 0;
}

int mem_slab_init(struct mem_slab *slab, void *buffer,
				  size_t block_size, uint32_t num_blocks)
{
	slab->num_blocks = num_blocks;
	slab->block_size = block_size;
	slab->buffer = buffer;
	slab->num_used = 0U;
	slab->max_used = 0U;
	int rc = create_free_list(slab);
	return rc;
}

int mem_slab_alloc(struct mem_slab *slab, void **mem)
{
	if (slab->free_list != NULL) {
		*mem = slab->free_list;//直接从空闲链表获得最后一项
		//更新空闲链表的索引
		slab->free_list = *(char **)(slab->free_list);
		slab->num_used++;
		slab->max_used = (slab->num_used > slab->max_used ? \
                          slab->num_used : slab->max_used);
        return 0;
    } else if (K_TIMEOUT_EQ(timeout, K_NO_WAIT)) {
        *mem = NULL;
        return -1;
    }
}

void mem_slab_free(struct mem_slab *slab, void **mem)
{
    **(char ***) mem = slab->free_list;
    slab->free_list = *(char **) mem;
    slab->num_used--;
    k_spin_unlock(&slab->lock, key);
}

#endif
