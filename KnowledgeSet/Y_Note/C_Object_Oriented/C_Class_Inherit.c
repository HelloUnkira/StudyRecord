/* 实例类定义: */
const C_Class C_Class_Inherit_Instance = {
    /* 如果有继承,指定对应基类的唯一实例类,如果没有继承,自己就是基类 */
    .BaseClass   = &C_Class_Base_Instance,
    .Constructor = C_Class_Inherit_Constructor,    /* 构造器回调钩子 */
    .Destructor  = C_Class_Inherit_Destructor,     /* 析构器回调钩子 */
    .Size        = sizeof(C_Inherit_base),         /* 实例类大小 */
};