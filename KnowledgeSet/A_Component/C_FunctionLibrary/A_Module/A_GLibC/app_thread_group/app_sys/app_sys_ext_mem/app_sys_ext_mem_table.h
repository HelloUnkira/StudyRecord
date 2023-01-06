#ifdef APP_SYS_EXT_MEM_H
#ifdef APP_SYS_EXT_MEM_TABLE_H

/*一个代替注册的,本地静态的外存chunk表
 *其中部分chunk属性在初始化是静态生成
 *注意:这个文件不可以以任何方式被其他地方引用
 *     它的唯一使用者为app_sys_ext_mem.c
 */

#define APP_SYS_EXT_MEM_CHUNK_BASE  0x00000000
#define APP_SYS_EXT_MEM_CHUNK_UNIT  4096

static const app_sys_ext_mem_t app_sys_ext_mem_table[] = {
    #define APP_SYS_EXT_MEM_CHUNK_BASE0     APP_SYS_EXT_MEM_CHUNK_BASE
    #define APP_SYS_EXT_MEM_CHUNK_SIZE0     APP_SYS_EXT_MEM_CHUNK_UNIT * 10
    {   /* chunk0 */
        .chunk_name = "chunk0",
        .chunk_base = APP_SYS_EXT_MEM_CHUNK_BASE0,
        .chunk_size = APP_SYS_EXT_MEM_CHUNK_SIZE0,
    },
    #define APP_SYS_EXT_MEM_CHUNK_BASE1     APP_SYS_EXT_MEM_CHUNK_BASE0 + APP_SYS_EXT_MEM_CHUNK_SIZE0
    #define APP_SYS_EXT_MEM_CHUNK_SIZE1     APP_SYS_EXT_MEM_CHUNK_UNIT * 20
    {   /* chunk1 */
        .chunk_name = "chunk1",
        .chunk_base = APP_SYS_EXT_MEM_CHUNK_BASE1,
        .chunk_size = APP_SYS_EXT_MEM_CHUNK_SIZE1,
    },
    #define APP_SYS_EXT_MEM_CHUNK_BASE2     APP_SYS_EXT_MEM_CHUNK_BASE1 + APP_SYS_EXT_MEM_CHUNK_SIZE1
    #define APP_SYS_EXT_MEM_CHUNK_SIZE2     APP_SYS_EXT_MEM_CHUNK_UNIT * 30
    {   /* chunk2 */
        .chunk_name = "chunk2",
        .chunk_base = APP_SYS_EXT_MEM_CHUNK_BASE2,
        .chunk_size = APP_SYS_EXT_MEM_CHUNK_SIZE2,
    },
};

#endif
#endif
