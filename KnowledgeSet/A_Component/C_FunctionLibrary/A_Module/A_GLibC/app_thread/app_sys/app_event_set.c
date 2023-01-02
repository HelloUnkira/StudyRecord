
#include "app_event_set.h"

extern void app_event_set_auxiliary_respond(app_event_set *event);

/* APP_EVENT1 */
extern void app_event_1_function_callback1(app_event_set *event);
extern void app_event_1_function_callback2(app_event_set *event);
extern void app_event_1_function_callback3(app_event_set *event);

/* APP_EVENT2 */
extern void app_event_2_function_callback1(app_event_set *event);
extern void app_event_2_function_callback2(app_event_set *event);


extern void app_event_set_respond(app_event_set *event)
{
	switch (event->command) {
	case APP_EVENT1:
		app_event_1_function_callback1(event);
		app_event_1_function_callback2(event);
		app_event_1_function_callback3(event);
		break;
	case APP_EVENT2:
		app_event_2_function_callback1(event);
		app_event_2_function_callback2(event);
		break;
	}
	if (event->auxiliary != 0)
		app_event_set_auxiliary_respond(event);
}
