#ifndef APP_MODULE_SOURCE_H
#define APP_MODULE_SOURCE_H

typedef {
    char     *data_name;
    uintptr_t data_base;
    uintptr_t data_size;
} app_module_source_t;

/*@brief     通过名字索引data静态实例
 *@param[in] name 名字
 *@retval    data静态实例
 */
const app_module_ext_mem_t * app_module_source_find_by_name(char *name);

/*@brief     通过地址索引data静态实例
 *@param[in] addr 地址
 *@retval    data静态实例
 */
const app_module_ext_mem_t * app_module_source_find_by_addr(uintptr_t addr);


#endif
