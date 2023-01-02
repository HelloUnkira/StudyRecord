#ifndef APP_SYS_EVENT_H
#define APP_SYS_EVENT_H

#include <stdint.h>

typedef struct {
	/* ���ݽ���ȡ����˫��Լ���׳� */
	uint32_t command;   /* ��Ӧ��ָ�� */
	uint32_t size;      /* ����Ԫ���� */
	void    *data;      /* ����Ԫ */
} app_sys_event;

extern void app_sys_event_respond(app_sys_event *event);

#define APP_EVENT1		0x0001
#define APP_EVENT2		0x0002

#endif
