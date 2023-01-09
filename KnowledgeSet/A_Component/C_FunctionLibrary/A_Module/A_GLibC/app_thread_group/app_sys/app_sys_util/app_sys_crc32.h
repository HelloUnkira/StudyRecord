#ifndef APP_SYS_CRC32_H
#define APP_SYS_CRC32_H

/*@brief     计算数据流的crc32校验
 *@param[in] data 数据流
 #@param[in] size 数据大小
 */
uint32_t app_sys_crc32(uint8_t *data, uint32_t size);

#endif
