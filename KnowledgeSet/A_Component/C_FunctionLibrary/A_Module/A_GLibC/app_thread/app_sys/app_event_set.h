#ifndef APP_EVENT_SET_H
#define APP_EVENT_SET_H

#include <stdint.h>

typedef struct {

	/* 如果需要善后,设置标签为1,否则为0 */
	/* 当事件的响应(0-n)全部执行完毕后 */
	/* 使用回调对数据进行做最终处理 */
	uint32_t command;   /* 响应的指令 */
	uint32_t auxiliary; /* 善后措施 */
	uint32_t length;    /* 数据元长度 */
	void    *data;      /* 数据元 */

} app_event_set;

extern void app_event_set_respond(app_event_set *event);

#define APP_EVENT1      0x0001
#define APP_EVENT2      0x0002

#endif
