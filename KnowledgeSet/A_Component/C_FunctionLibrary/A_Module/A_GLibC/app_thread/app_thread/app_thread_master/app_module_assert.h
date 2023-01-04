#ifndef APP_MODULE_ASSERT_H
#define APP_MODULE_ASSERT_H

void app_module_assert(char *file, uint32_t line, bool condition);

/* 断言:条件为真就绪执行,为假中断 */
#define APP_MODULE_ASSERT(condition)    \
        app_module_assert(__FILE__, __LINE__, condition)

#endif
