
#ifndef __JERRYSCRIPT_PACKAGE_H__
#define __JERRYSCRIPT_PACKAGE_H__

// C标准库
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// JerryScript系统库接口
#include "jerryscript.h"

// 平台配置项





/*实现目标:
 *对Jerryscript的库进行简要封装
 */

// 内存分配策略
#define  JS_ZEPHYR     1
// 
#if JS_ZEPHYR
//zephyr平台
#include <zephyr.h>
#define js_malloc(sz)                                      \
({                                                          \
    extern void *   js_malloc_with_retry(size_t size);     \
    void *zjs_ptr = js_malloc_with_retry(sz);              \
    if (!zjs_ptr) {                                         \
        DEBUG_PRINT("malloc(%u) failed\n", (uint32_t)(sz)); \
    }                                                       \
    zjs_ptr;                                                \
})
#define js_free(ptr) TFREE(ptr)
//其余平台
//#elif JS_XXX
#else
#define js_malloc(sz) TMALLOC(sz)
#define js_free(ptr)  TFREE(ptr)
#endif

// JerryScript系统库接口封装:
typedef struct js_native_func {
          void *function;
    const char *name;
} js_native_func_t;

//基本库接口封装:
bool js_delete_property(const jerry_value_t object, const char *name);
jerry_value_t js_get_property(const jerry_value_t object, const char *name);
void js_set_property(const jerry_value_t object, const jerry_value_t property, const char *name);
void js_set_readonly_property(const jerry_value_t object, const jerry_value_t property, const char *name);
//扩充属性封装:
void js_object_add_boolean(jerry_value_t object, const char *name, bool flag);
void js_object_add_string(jerry_value_t object, const char *name, const char *string);
void js_object_add_number(jerry_value_t object, const char *name, double number);
void js_object_add_readonly_boolean(jerry_value_t object, const char *name, bool flag);
void js_object_add_readonly_string(jerry_value_t object, const char *name, const char *string);
void js_object_add_readonly_number(jerry_value_t object, const char *name, double number);
bool js_object_get_boolean(jerry_value_t object, const char *name, bool *flag);
bool js_object_get_string(jerry_value_t object, const char *name, char *buffer, int length);
bool js_object_get_double(jerry_value_t object, const char *name, double *number);
//这个函数可以不要,double可以无损坏的截断为uint32_t或int32_t,直接使用上面的接口就行了
bool js_object_get_uint32(jerry_value_t object, const char *name, uint32_t *number);
//这个函数可以不要,double可以无损坏的截断为int32,直接使用上面的接口就行了
bool js_object_get_int32(jerry_value_t object, const char *name, int32_t *number);
//扩充属性封装:
void js_object_add_function(jerry_value_t object, const char *name, void *function);
void js_object_add_functions(jerry_value_t object, js_native_func_t *funcs);
void js_object_add_object(jerry_value_t parent, jerry_value_t child, const char *name);
//扩充属性封装:
void js_release_value_callback(const jerry_value_t *value);
jerry_value_t js_array_push(jerry_value_t array, jerry_value_t value);
void js_string_copy(jerry_value_t jerry_string, char *buffer, jerry_size_t *length);
char *js_string_get(jerry_value_t jerry_string, jerry_size_t *length);
void js_nop_free_callback(void *h, struct jerry_object_native_info_t* info_p);

// 内部使用的,带编译挂载回收机制的宏
#define js_cvalue_t  const jerry_value_t __attribute__((__cleanup__(js_release_value_callback)))
#define js_value_t         jerry_value_t __attribute__((__cleanup__(js_release_value_callback)))

//优化代码编写策略

//定义(JS响应C函数时使用)(提供给JS调用)
#define JS_DECL_FUNC(name)                                  \
    jerry_value_t name(const jerry_call_info_t *call_info_p, \
                       const jerry_value_t      argv[],      \
                       const jerry_length_t     argc)

//定义(JS响应C函数时使用)(泛型,内部调用)
#define JS_DECL_FUNC_ARGS(name, ...)                        \
    jerry_value_t name(const jerry_call_info_t *call_info_p, \
                       const jerry_value_t      argv[],      \
                       const jerry_length_t     argc, __VA_ARGS__)

//引用(JS响应C函数时使用)(泛型,内部调用)
#define JS_CHAIN_FUNC_ARGS(name, ...) \
    name(call_info_p, argv, argc, __VA_ARGS__)

//单链表宏迭代器
#include "jerryscript_slist.h"

/**
 * 获取对象的本地句柄或空
 * 它声明一个类型'type'的指针变量'var'
 * 并将它设置为检索到的指针,如果它匹配给定的'info',否则为NULL
 * @param obj     合法的JS对象
 * @param type    本机句柄的类型名称
 * @param var     要声明的变量的名称
 * @param info    为这个本机句柄类型定义的jerry_object_native_info_t类型的名称
 */
#define JS_GET_HANDLE_OR_NULL(obj, type, var, info)               \
    type *var = NULL;                                              \
    {                                                              \
        void *native;                                              \
        const jerry_object_native_info_t *tmp = &info;             \
        if (jerry_get_object_native_pointer(obj, &native, tmp))	   \
		{														   \
            var = (type *)native;                                  \
        }                                                          \
    }

#endif

//Abbr(abbreviation):
//DECL:declare
