#ifndef APP_ARCH_EXT_MEM_H
#define APP_ARCH_EXT_MEM_H

/* ext mem */
#define APP_ARCH_EXT_MEM    0
#ifdef  APP_ARCH_EXT_MEM

typedef struct {
    char     *chunk_name;   /* 使用name读写chunk(支持fs模式时) */
    uintptr_t chunk_base;   /* 使用base读写chunk(支持raw模式时) */
    ssize_t   chunk_size;   /* chunk大小 */
} app_arch_ext_mem_t;

/*@brief             读取数据从指定的文件中
 *@param[in] ext_mem 文件静态实例
 *@param[in] offset  文件偏移
 *@param[in] buffer  数据缓存
 *@param[in] size    数据缓存大小
 *@retval            读取数据实际大小(失败返回负数)
 */
ssize_t app_arch_ext_mem_read(const app_arch_ext_mem_t *ext_mem, uintptr_t offset, uint8_t *buffer, size_t size);

/*@brief             写入数据到指定的文件中
 *@param[in] ext_mem 文件静态实例
 *@param[in] offset  文件偏移
 *@param[in] buffer  数据缓存
 *@param[in] size    数据缓存大小
 *@retval            写入数据实际大小(失败返回负数)
 */
ssize_t app_arch_ext_mem_write(const app_arch_ext_mem_t *ext_mem, uintptr_t offset, uint8_t *buffer, size_t size);

/*@brief  初始化外存及文件系统
 *@retval 失败返回负数
 */
int32_t app_arch_ext_mem_ready(void);

#endif

#endif
