/*
 *外部引用,显式引用
 *因为依赖是脚本提供的
 *所以不采用引入头文件的方式
 */
#include "EventSet.h"

extern void EventSetAuxiliaryRespond(EventSet *Event);
/* EVENT1 */
extern void Event1FunctionCallback1(EventSet *Event);
extern void Event1FunctionCallback2(EventSet *Event);
extern void Event1FunctionCallback3(EventSet *Event);
/* EVENT2 */
extern void Event2FunctionCallback1(EventSet *Event);
extern void Event2FunctionCallback2(EventSet *Event);


extern void EventSetRespond(EventSet *Event)
{
	switch (Event->Command) {
	case EVENT1:
		Event1FunctionCallback1(Event);
		Event1FunctionCallback2(Event);
		Event1FunctionCallback3(Event);
		break;
	case EVENT2:
		Event2FunctionCallback1(Event);
		Event2FunctionCallback2(Event);
		break;
	}
	if (Event->Auxiliary != 0)
		EventSetAuxiliaryRespond(Event);
}
