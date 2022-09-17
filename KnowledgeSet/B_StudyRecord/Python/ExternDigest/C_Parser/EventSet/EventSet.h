
#ifndef EVENT_SET_H
#define EVENT_SET_H
//C std lib
#include <stdint.h>

typedef struct EventSetBody {

	/* 如果需要善后,设置标签为1,否则为0 */
	/* 当事件的响应(0-n)全部执行完毕后 */
	/* 使用回调对数据进行做最终处理 */
	uint32_t Command;   /* 响应的指令 */
	uint32_t Auxiliary; /* 善后措施 */
	uint32_t Length;    /* 数据元长度 */
	void    *Data;      /* 数据元 */

} EventSet;

extern void EventSetRespond(EventSet *Event);

#define EVENT1		0x0001
#define EVENT2		0x0002

#endif
