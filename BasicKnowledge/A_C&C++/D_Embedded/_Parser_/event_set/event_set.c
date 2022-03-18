/*
 *外部引用,显式引用
 *因为依赖是脚本提供的
 *所以不采用引入头文件的方式
 */
#include "event_set.h"

extern void event_set_auxiliary_respond(event_set *event);
/* EVENT1 */
/* this is response set: */
extern void event1_function_callback1(event_set *event);
extern void event1_function_callback2(event_set *event);
extern void event1_function_callback3(event_set *event);
/* EVENT2 */
/* this is response set: */
extern void event2_function_callback1(event_set *event);
extern void event2_function_callback2(event_set *event);


extern void event_set_respond(event_set *event)
{
	switch (event->command) {
	case EVENT1 :
		event1_function_callback1(event);
		event1_function_callback2(event);
		event1_function_callback3(event);
		break;
	case EVENT2 :
		event2_function_callback1(event);
		event2_function_callback2(event);
		break;
	}
	if (event->auxiliary != 0)
		event_set_auxiliary_respond(event);
}
