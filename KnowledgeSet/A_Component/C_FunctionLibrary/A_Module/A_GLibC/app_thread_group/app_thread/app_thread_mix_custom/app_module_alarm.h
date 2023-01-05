#ifndef APP_MODULE_ALARM_H
#define APP_MODULE_ALARM_H

typedef struct {
    app_module_clock_t clock_base;  /* 设置时的时钟 */
    app_module_clock_t clock_month; /* 月更新提醒时钟(内部维护) */
    app_module_clock_t clock_week;  /* 周更新提醒时钟(内部维护) */
    uint32_t snooze:6;              /* 贪睡时长(分钟), 0为不贪睡 */
    uint16_t field_month:12;        /* 一月到十二月 */
    uint8_t  field_week:7;          /* 日一二三四五六 */
    uint8_t  valid:1;               /* 有效性 */
    uint8_t  onoff:1;               /* 启停状态 */
    /* 使用者自行维护字段 */
    uint8_t  silence:1;             /* 静默提醒 */
    char    *name;                  /* 闹钟名字 */
} app_module_alarm_t;

typedef struct {
    app_module_alarm_t *array;      /* 闹钟数组 */
    uint32_t            number;     /* 闹钟总数量 */
    uint32_t            used;       /* 闹钟使用数量 */
    app_mutex_t         mutex;      /* 闹钟组资源保护 */
} app_module_alarm_group_t;

/* 本地闹钟组限制 */
#define APP_MODULE_ALARM_GROUP_MAX  5

/*@brief 初始化闹钟组
 */
void app_module_alarm_group_ready(void);

/*@brief        注册闹钟组
 *@param[in]    array  闹钟数组
 *@param[in]    number 闹钟数量
 *@retval       闹钟组id(错误为-1)
 */
uint32_t app_module_alarm_group_register(app_module_alarm_t *array, uint32_t number);

/*@brief        闹钟组添加闹钟
 *@param[in]    alarm_group_id 闹钟组id
 *@param[in]    alarm          闹钟实例
 *@retval       闹钟id(错误为-1)
 */
uint32_t app_module_alarm_add(uint32_t alarm_group_id, app_module_alarm_t *alarm);

/*@brief        闹钟组启动闹钟
 *@param[in]    alarm_group_id 闹钟组id
 *@param[in]    alarm          闹钟实例
 */
void app_module_alarm_start(uint32_t alarm_group_id, uint32_t alarm_id);

/*@brief        闹钟组停止闹钟
 *@param[in]    alarm_group_id 闹钟组id
 *@param[in]    alarm          闹钟实例
 */
void app_module_alarm_stop(uint32_t alarm_group_id, uint32_t alarm_id);

/*@brief        闹钟组移除闹钟
 *@param[in]    alarm_group_id 闹钟组id
 *@param[in]    alarm_id       闹钟实例
 */
void app_module_alarm_del(uint32_t alarm_group_id, uint32_t alarm_id);

/*@brief        设置闹钟组中的闹钟实例
 *@param[in]    alarm_group_id 闹钟组id
 *@param[in]    alarm_id       闹钟id
 *@param[out]   alarm          闹钟实例
 */
void app_module_alarm_set(uint32_t alarm_group_id, uint32_t alarm_id, app_module_alarm_t *alarm);

/*@brief        获得闹钟组中的闹钟实例
 *@param[in]    alarm_group_id 闹钟组id
 *@param[in]    alarm_id       闹钟id
 *@param[out]   alarm          闹钟实例
 */
void app_module_alarm_get(uint32_t alarm_group_id, uint32_t alarm_id, app_module_alarm_t *alarm);

#endif
