#ifdef APP_MODULE_CLOCK_CB_H
#ifdef APP_MODULE_CLOCK_H

/*实现目标:
 *一个代替动态注册的,静态本地导入的时钟秒时分天回调集
 *它会被内置到mix custom线程中作为一个辅助功能使用
 *秒时分天回调会在到点时执行下面注册的回调,按顺序执行
 *更新回调则当本地时钟被更新时调用,辅助上层更新对应的事项
 *因为这些回调都是编码时注册,本地对应回调的实际空间占用
 *最终编译器都会将其转化为函数代码段函数表,不占用内存
 *注意:这个文件不可以以任何方式被其他地方引用
 *     它的唯一使用者为app_module_clock.c
 */

/* 函数声明及其引用(注意:不要导入头文件,直接提供函数名即可): */
/* extern void xxxx(app_module_clock_t *clock); */

static void app_module_clock_cb1_empty(app_module_clock_t *clock)
{
    APP_OS_PRINT("app_module_clock_cb1_empty\n");
}

static void app_module_clock_cb2_empty(app_module_clock_t *last_clock,
                                       app_module_clock_t *curr_clock, uint32_t event)
{
    APP_OS_PRINT("app_module_clock_cb2_empty\n");
}

/* 秒回调 */
const app_module_clock_cb1 app_module_clock_second_cb[] = {
    app_module_clock_cb1_empty,
    /* 补充中...... */
};

/* 分回调 */
const app_module_clock_cb1 app_module_clock_minute_cb[] = {
    app_module_clock_cb1_empty,
    /* 补充中...... */
};

/* 时回调 */
const app_module_clock_cb1 app_module_clock_hour_cb[] = {
    app_module_clock_cb1_empty,
    /* 补充中...... */
};

/* 天回调 */
const app_module_clock_cb1 app_module_clock_day_cb[] = {
    app_module_clock_cb1_empty,
    /* 补充中...... */
};

/* 更新回调 */
const app_module_clock_cb2 app_module_clock_update_cb[] = {
    app_module_clock_cb2_empty,
    /* 补充中...... */
};

#endif
#endif
