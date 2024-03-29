/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#include "C_Class.h"
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
typedef struct C_Class_Base_Body {
    /* 静态实例类指针:类型(C_Class *) */
    const void *Class;
    /* 基对象指针:类型(C_Object *) */
    void *Base;
    /* 挂载该实例对象的其他私有参数 */
    void *UserData;
    /* ... */
} C_Class_base;
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
static void C_Class_Base_Constructor(const void *Class, C_Class_base *Object)
{
    /* 初始化Object(如果需要动态申请空间,可以动态申请) */
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
static void C_Class_Base_Destructor(const void *Class, C_Class_base *Object)
{
    /* 反初始化Object(如果需要动态释放空间,需要动态释放) */
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 实例类定义: */
const C_Class C_Class_Base_Instance = {
    /* 如果有继承,指定对应基类的唯一实例类,如果没有继承,自己就是基类 */
    .BaseClass   = NULL,
    .Constructor = C_Class_Base_Constructor,    /* 构造器回调钩子 */
    .Destructor  = C_Class_Base_Destructor,     /* 析构器回调钩子 */
    .Size        = sizeof(C_Class_base),        /* 实例类大小 */
};
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 创建对象 */
void * C_Class_Base_Create(void *Base)
{
    /* 注意:这个过程可以是递归的,如果不提供它的基对象,那么 */
    /*      需要先递归的调用基类对象的创建函数生成基类对象 */
    /* 1.动态生成对象Object(C_Class_Base_Instance.Size) */
    /* 2.启用对象构造 */
    
    //对象构造
    if (Object->Constructor != NULL)
        Object->Constructor(C_Class_Base_Instance, Object);
    
    Object->Class = C_Class_Base_Instance;
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 销毁对象 */
void C_Class_Base_Destroy(void *Object)
{    
    /* 注意:这个过程可以是递归的,如果不提供它的基对象,那么 */
    /*      需要先递归的调用基类对象的创建函数生成基类对象 */
    /* 1.启用对象析构 */
    /* 2.动态清除对象Object(C_Class_Base_Instance.Size) */
    
    //对象析构
    if (Object->Destructor != NULL)
        Object->Destructor(C_Class_Base_Instance, Object);
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 各种Set和Get函数所组成的核心转义,用以达到数据隐藏 */
/* 此时所有的元素都类似于C++中的Private属性 */
void C_Class_Base_GetXXX(void *Object,...); // {...}
void C_Class_Base_SetXXX(void *Object,...); // {...}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
