#define SOURCE_FILE_C
#include "source_file.h"

///////////////////////////////////////////////////////////////////////////////
//操作集合的具体实现///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
static void xxx_implement_init(data_set* set)
{
    if (set == NULL)
        return;
    //操作set的init_arg
}

static void xxx_implement_deinit(data_set* set)
{
    if (set == NULL)
        return;
    //操作set的deinit_arg
}

static void xxx_implement_open(data_set* set)
{
    if (set == NULL)
        return;
    //操作set的open_arg
}

static void xxx_implement_close(data_set* set)
{
    if (set == NULL)
        return;
    //操作set的close_arg
}

static void xxx_implement_other(data_set* set)
{
    if (set == NULL)
        return;
    //操作set的other_arg
}



///////////////////////////////////////////////////////////////////////////////
//操作集合采用,单例模式////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
typedef struct source_operation_set {
    //所有extern的操作项
    void (*init)(data_set* set);
    void (*deinit)(data_set* set);
    void (*open)(data_set* set);
    void (*close)(data_set* set);
    void (*other)(data_set* set);
} operation_set;

///////////////////////////////////////////////////////////////////////////////
//必要的存在,单例模式//////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
static operation_set instance = { //注册操作集
    .init   = xxx_implement_init,
    .deinit = xxx_implement_deinit,
    .open   = xxx_implement_open,
    .close  = xxx_implement_close,
    .other  = xxx_implement_other
};

//注册操作集
static void register_operation_set(void)
{
    static int register_finished = 0;
    if (register_finished != 0)
        return;
    register_finished = 1;
    
    instance.init   = xxx_implement_init;
    instance.deinit = xxx_implement_deinit;
    instance.open   = xxx_implement_open;
    instance.close  = xxx_implement_close;
    instance.other  = xxx_implement_other;
}

///////////////////////////////////////////////////////////////////////////////
//操作集封装,统一接口//////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void xxx_init(data_set* data)
{
    register_operation_set();
    if (instance.init != NULL)
        instance.init(data);
}

void xxx_deinit(data_set* data)
{
    register_operation_set();
    if (instance.deinit != NULL)
        instance.deinit(data);
}

void xxx_open(data_set* data)
{
    register_operation_set();
    if (instance.open != NULL)
        instance.open(data);
}

void xxx_close(data_set* data)
{
    register_operation_set();
    if (instance.close != NULL)
        instance.close(data);
}

void xxx_other(data_set* data)
{
    register_operation_set();
    if (instance.other != NULL)
        instance.other(data);
}



