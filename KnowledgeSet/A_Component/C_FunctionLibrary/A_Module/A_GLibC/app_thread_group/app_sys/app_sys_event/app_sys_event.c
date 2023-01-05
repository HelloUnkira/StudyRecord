/*
 *外部引用,显式引用
 *因为依赖是脚本提供的
 *所以不采用引入头文件的方式
 */
#include "app_sys_event.h"

/* APP_EVENT1 */
/* this is response set: */
extern void event1_function_callback1(app_sys_event *event);
extern void event1_function_callback2(app_sys_event *event);
extern void event1_function_callback3(app_sys_event *event);
/* APP_EVENT2 */
/* this is response set: */
extern void event2_function_callback1(app_sys_event *event);
extern void event2_function_callback2(app_sys_event *event);

extern void app_sys_event_respond(app_sys_event *event)
{
	switch (event->command) {
	case APP_EVENT1:
		event1_function_callback1(event);
		event1_function_callback2(event);
		event1_function_callback3(event);
		break;
	case APP_EVENT2:
		event2_function_callback1(event);
		event2_function_callback2(event);
		break;
	}
}
