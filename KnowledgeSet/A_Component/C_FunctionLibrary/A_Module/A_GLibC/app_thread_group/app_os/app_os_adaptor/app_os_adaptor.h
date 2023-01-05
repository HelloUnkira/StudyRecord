#ifndef APP_OS_ADAPTOR_H
#define APP_OS_ADAPTOR_H

/* APP OS adaptor: */

#define APP_OS_IS_LINUX     1
#define APP_OS_IS_ZEPHYR    0
#define APP_OS_IS_FREERTOS  0

#if 0
#elif APP_OS_IS_LINUX
#include "app_os_adaptor_linux.h"
#elif APP_OS_IS_ZEPHYR
#include "app_os_adaptor_zephyr.h"
#elif APP_OS_IS_FREERTOS
#include "app_os_adaptor_freertos.h"
#else
#error "unknown os"
#endif

/* 全局宏控开关及其等级 */
#define   APP_OS_LOG_GLOBAL_STATUS  1   /* 1:ENABLE,0:DISABLE */
#include "app_os_log.h"
#include "app_os_build_time.h"

#endif
