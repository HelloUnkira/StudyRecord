#ifndef APP_MODULE_SOURCE_H
#define APP_MODULE_SOURCE_H

typedef struct {
    char     *data_name;
    uintptr_t data_base;
    uintptr_t data_size;
} app_module_source_t;

#endif
