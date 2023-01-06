#ifdef APP_MODULE_EXT_MEM_H
#ifdef APP_MODULE_EXT_MEM_TABLE_H

/*一个代替注册的,本地静态的外存chunk表
 *注意:这个文件不可以以任何方式被其他地方引用
 *     它的唯一使用者为app_module_ext_mem.c
 */

#define APP_MODULE_EXT_MEM_CHUNK_BASE  0x00000000
#define APP_MODULE_EXT_MEM_CHUNK_UNIT  4096

static const app_module_ext_mem_t app_module_ext_mem_table[] = {
    #define APP_MODULE_EXT_MEM_CHUNK_BASE0     APP_MODULE_EXT_MEM_CHUNK_BASE
    #define APP_MODULE_EXT_MEM_CHUNK_SIZE0     APP_MODULE_EXT_MEM_CHUNK_UNIT * 10
    {   /* chunk0:thread_master */
        .chunk_name = "thread_master",
        .chunk_base = APP_MODULE_EXT_MEM_CHUNK_BASE0,
        .chunk_size = APP_MODULE_EXT_MEM_CHUNK_SIZE0,
    },
    #define APP_MODULE_EXT_MEM_CHUNK_BASE1     APP_MODULE_EXT_MEM_CHUNK_BASE0 + APP_MODULE_EXT_MEM_CHUNK_SIZE0
    #define APP_MODULE_EXT_MEM_CHUNK_SIZE1     APP_MODULE_EXT_MEM_CHUNK_UNIT * 10
    {   /* chunk1:thread_mix_irq */
        .chunk_name = "thread_mix_irq",
        .chunk_base = APP_MODULE_EXT_MEM_CHUNK_BASE1,
        .chunk_size = APP_MODULE_EXT_MEM_CHUNK_SIZE1,
    },
    #define APP_MODULE_EXT_MEM_CHUNK_BASE2     APP_MODULE_EXT_MEM_CHUNK_BASE1 + APP_MODULE_EXT_MEM_CHUNK_SIZE1
    #define APP_MODULE_EXT_MEM_CHUNK_SIZE2     APP_MODULE_EXT_MEM_CHUNK_UNIT * 100
    {   /* chunk2:thread_mix_custom */
        .chunk_name = "thread_mix_custom",
        .chunk_base = APP_MODULE_EXT_MEM_CHUNK_BASE2,
        .chunk_size = APP_MODULE_EXT_MEM_CHUNK_SIZE2,
    },
};

#endif
#endif
