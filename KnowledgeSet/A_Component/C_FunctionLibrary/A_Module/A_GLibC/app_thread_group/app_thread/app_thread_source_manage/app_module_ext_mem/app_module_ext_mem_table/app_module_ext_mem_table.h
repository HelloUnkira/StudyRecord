#ifndef APP_MODULE_EXT_MEM_TABLE_H
#define APP_MODULE_EXT_MEM_TABLE_H

/*一个代替注册的,脚本自动生成的,本地静态的外存chunk表
 *通过app_module_ext_mem_table.py生成
 *参考app_module_ext_mem_table.json中的模式生成源
 */

/*@brief通过名字索引chunk静态实例
 *@param[in] name名字
 *@retval    chunk静态实例
 */
const app_module_ext_mem_t * app_module_ext_mem_find_by_name(const char *name);

#endif
