// Copyright (c) 2016-2018, Intel Corporation.

#include "jerryscript_package.h"

/*实现目标:
 *对jerry的库进行封装
 *注意:
 *约定俗成
 *根据高级语言的特性,所有类型都是对象
 *下述中的指定类型的值变量都统一叫做xxx值对象
 */

//一个用于回收require调用产生的资源的回调
void js_release_value_callback(const jerry_value_t *value)
{
    jerry_release_value(*value);
}

//尝试搭配垃圾回收机制去进行内存分配
void * js_malloc_with_retry(size_t size)
{
    void *ptr = TMALLOC(size);
    if (!ptr) {
        //查看过时的JerryScript对象是否占用内存
        jerry_gc(JERRY_GC_PRESSURE_LOW);
        ptr = TMALLOC(size);
    }
    return ptr;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//基本属性操作

//在名为name的对象中创建一个新字段,并为其设置property
void js_set_property(const jerry_value_t object, const jerry_value_t property, const char *name)
{
    //1.申请一个字符串对象
    js_cvalue_t jerry_name = jerry_create_string((const jerry_char_t *)name);
    //2.为对象挂载属性
    jerry_set_property(object, jerry_name, property);
}

//在名为name的对象中创建一个新的只读字段,并为其设置property
void js_set_readonly_property(const jerry_value_t object, const jerry_value_t property, const char *name)
{
    //1.申请一个字符串对象
    js_cvalue_t jerry_name = jerry_create_string((const jerry_char_t *)name);
    //2.申请一个属性描述符对象
    jerry_property_descriptor_t property_descriptor = {0};
    property_descriptor = jerry_property_descriptor_create();
    //3.属性描述符对象挂载属性
    property_descriptor.value = property;
    //4.为对象挂载属性
    jerry_define_own_property(object, jerry_name, &property_descriptor);
    //5.释放一个属性描述符对象
    jerry_property_descriptor_free(&property_descriptor);
}

//在object中查找属性名,并返回它;该值将由调用者拥有,并且必须释放
jerry_value_t js_get_property(const jerry_value_t object, const char *name)
{
    //1.申请一个字符串对象
    js_cvalue_t jerry_name = jerry_create_string((const jerry_char_t *)name);
    //2.查找对象指定属性并返回
    return jerry_get_property(object, jerry_name);
}

//在object中查找属性名,并删除它;如果成功返回true,否则返回false
bool js_delete_property(const jerry_value_t object, const char *name)
{
    //1.申请一个字符串对象
    js_cvalue_t jerry_name = jerry_create_string((const jerry_char_t *)name);
    //2.为对象移除指定属性
    return jerry_delete_property(object, jerry_name);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//扩充属性操作

//在父字段name中创建一个新字段,设置为value
void js_object_add_boolean(jerry_value_t object, const char *name, bool flag)
{
    //1.申请一个字符串对象
    js_cvalue_t jerry_name = jerry_create_string((const jerry_char_t *)name);
    //2.申请一个布尔对象
    js_cvalue_t jerry_bool = jerry_create_boolean(flag);
    //3.为对象挂载属性
    jerry_set_property(object, jerry_name, jerry_bool);
}

//在父字段name中创建一个新字段，设置为string
void js_object_add_string(jerry_value_t object, const char *name, const char *string)
{
    //1.申请一个字符串对象
    js_cvalue_t jerry_name = jerry_create_string((const jerry_char_t *)name);
    //2.申请一个字符串对象
    js_cvalue_t jerry_string = jerry_create_string((const jerry_char_t *)string);
    //3.为对象挂载属性
    jerry_set_property(object, jerry_name, jerry_string);
}

//在父字段name中创建一个新字段,设置为value
void js_object_add_number(jerry_value_t object, const char *name, double number)
{   
    //1.申请一个字符串对象
    js_cvalue_t jerry_name = jerry_create_string((const jerry_char_t *)name);
    //2.申请一个数字对象
    js_cvalue_t jerry_number = jerry_create_number(number);
    //3.为对象挂载属性
    jerry_set_property(object, jerry_name, jerry_number);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//扩充属性操作

bool js_object_get_boolean(jerry_value_t object, const char *name, bool *flag)
{
    //1.获取一个布尔对象
    js_cvalue_t jerry_bool = js_get_property(object, name);
    //2.对象属性检查
    if (jerry_value_is_error(jerry_bool))
        return false;
    if (jerry_value_is_boolean(jerry_bool))
        return false;
    //3.获取属性
    *flag = jerry_value_is_true(jerry_bool);
    return true;
}

bool js_object_get_string(jerry_value_t object, const char *name, char *buffer, int length)
{
    //1.获取一个字符串对象
    js_cvalue_t jerry_string = zjs_get_property(object, name);
    //2.对象属性检查
    if (jerry_value_is_error(jerry_string))
        return false;
    if (jerry_value_is_string(jerry_string))
        return false;
    //3.获取属性
    js_copy_jerry_string(jerry_string, buffer, &length);
    //4.最后检查
    if (length != 0)
        return true;
    if (length == 0)
        return false;
}

bool js_object_get_double(jerry_value_t object, const char *name, double *number)
{
    //1.获取一个数值对象
    js_cvalue_t jerry_number = zjs_get_property(object, name);
    //2.对象属性检查
    if (jerry_value_is_error(jerry_number))
        return false;
    if (jerry_value_is_number(jerry_number))
        return false;
    //3.获取属性
    *number = jerry_get_number_value(jerry_number);
    return true;
}

//这个函数可以不要,double可以无损坏的截断为uint32,直接使用上面的接口就行了
bool js_object_get_uint32(jerry_value_t object, const char *name, uint32_t *number)
{
    //1.获取一个数值对象
    js_cvalue_t jerry_number = zjs_get_property(object, name);
    //2.对象属性检查
    if (jerry_value_is_error(jerry_number))
        return false;
    if (jerry_value_is_number(jerry_number))
        return false;
    //3.获取属性
    *number = jerry_get_number_value(jerry_number);
    return true;
}

//这个函数可以不要,double可以无损坏的截断为uint32,直接使用上面的接口就行了
bool js_object_get_int32(jerry_value_t object, const char *name, int32_t *number)
{
    //1.获取一个数值对象
    js_cvalue_t jerry_number = zjs_get_property(object, name);
    //2.对象属性检查
    if (jerry_value_is_error(jerry_number))
        return false;
    if (jerry_value_is_number(jerry_number))
        return false;
    //3.获取属性
    double value = jerry_get_number_value(jerry_number);
    *number = (int32_t)value;
    return true;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//扩充属性操作

//向一个数组对象压入一个值
jerry_value_t js_array_push(jerry_value_t array, jerry_value_t value)
{
    //对象属性检查
    //对象不是一个数组
    if (jerry_value_is_array(array) == false) {
        //1.对象不是一个数组,获取对象长度
        uint32_t size = jerry_get_array_length(array);
        //2.临时创建一个数组对象
        jerry_value_t new_array = jerry_create_array(size + 1);
        //3.将旧对象数据拷贝到新数组对象中
        for (int i = 0; i < size; i++) {
            //1.获得对应索引的值
            js_cvalue_t temp = jerry_get_property_by_index(array, i);
            //2.将各个值加入到新数组对象中
            jerry_set_property_by_index(new_array, i, temp);
        }
        //4.尾部追加一个新的数据
        jerry_set_property_by_index(new_array, size, value);
        //5.返回一个新的数组对象
        return new_array;
    }
    //对象是一个数组
    if (jerry_value_is_array(array) == true) {
        //1.临时创建一个数组对象
        jerry_value_t new_array = jerry_create_array(1);
        //2.设置这个数组对象的数据
        jerry_set_property_by_index(new_array, 0, value);
        //3.返回一个新的数组对象
        return new_array;
    }
}

//从字符串对象中拷贝数据
void js_string_copy(jerry_value_t jerry_string, char *buffer, jerry_size_t *length)
{
    //1.获得字符串占用空间
    jerry_size_t size = jerry_get_string_size(jerry_string);
    //2.从字符串提取字符数据
    jerry_size_t len = 0;
    if (*length > size)
        len = jerry_string_to_utf8_char_buffer(jerry_string, (jerry_char_t *)buffer, size);
    
    *length = len;
    buffer[len] = '\0';
}

//从字符串对象中获取数据(生成动态空间)
char *js_string_get(jerry_value_t jerry_string, jerry_size_t *length)
{
    //1.获得字符串占用空间
    jerry_size_t size = jerry_get_string_size(jerry_string);
    //2.获取缓冲区
    char *buffer = js_malloc(size + 1);
    //3.检查
    if (buffer == NULL)
        return NULL;
    //4.拷贝整个字符串
    jerry_size_t len = jerry_string_to_utf8_char_buffer(jerry_string, (jerry_char_t *)buffer, size);
    buffer[len] = '\0';
    //5.
    if (length != NULL) {
        if (*length && *length < len) {
            buffer[*length] = '\0';
        } else {
            *length = len;
        }
    }
    return buffer;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//扩充属性操作

//在对象中创建一个名为name的新字段,这将是一个调用给定C函数的JS函数
void js_object_add_function(jerry_value_t object, const char *name, void *function)
{
    //1.申请一个字符串对象
    js_cvalue_t jerry_name = jerry_create_string((const jerry_char_t *)name);
    //2.申请一个函数对象
    js_cvalue_t jerry_function = jerry_create_external_function(function);
    //3.检查这个函数对象是否创建成功
    if (jerry_value_is_function(jerry_function) == false)
        return;
    //4.为对象挂载属性
    jerry_set_property(object, jerry_name, jerry_function);
}

//在父字段name中创建一个引用子字段的新字段
void js_object_add_object(jerry_value_t parent, jerry_value_t child, const char *name)
{
    //1.申请一个字符串对象
    js_cvalue_t jerry_name = jerry_create_string((const jerry_char_t *)name);
    //2.为对象挂载属性
    jerry_set_property(parent, jerry_name, child);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//简化措施:

//在父字段name中创建一个新的只读字段,设置为value
void js_object_add_readonly_boolean(jerry_value_t object, const char *name, bool flag)
{
    js_set_readonly_property(object, name, jerry_create_boolean(flag));
}

//在父字段name中创建一个新的只读字段,设置为string
void js_object_add_readonly_string(jerry_value_t object, const char *name, const char *string)
{
    js_set_readonly_property(object, name, jerry_create_string((const jerry_char_t *)string));
}

//在父字段name中创建一个新的只读字段,设置为number
void js_object_add_readonly_number(jerry_value_t object, const char *name, double number)
{
    js_set_readonly_property(object, name, jerry_create_number(number));
}

//将所有描述的函数以给定的名称添加到object中
void js_object_add_functions(jerry_value_t object, js_native_func_t *funcs)
{
    for (js_native_func_t *map = funcs; map->function; map++) {
        js_object_add_function(object, map->name, map->function);
    }
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//其他扩充操作

void js_nop_free_callback(void *h, struct jerry_object_native_info_t* info_p)
{
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
