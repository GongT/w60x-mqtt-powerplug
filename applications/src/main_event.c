#include "app.h"
#include <stdlib.h>
#include <easyflash.h>
#include <gongt/config_tool.h>

typedef struct main_event_s {
	enum main_event_kind kind;
	rt_bool_t free_payload;
	const char *payload;
} main_event;

static rt_mq_t main_events = NULL;

void main_event_queue(enum main_event_kind kind, const char *body, rt_bool_t copy_mem) {
	if (main_events == NULL) {
		KPRINTF_COLOR(11, "main_event_queue: ignore: app not init");
		return;
	}

	main_event evt = {kind, copy_mem, copy_mem ? strdup(body) : body};

	int r = rt_mq_send(main_events, (void *)&evt, sizeof(main_event));
	if (r != RT_EOK)
		KPRINTF_COLOR(11, "rt_mq_send[%d]: return %d", kind, r);
}

__attribute__((noreturn)) void start_main_event_loop() {
	main_events = rt_mq_create("main", sizeof(main_event), 5, RT_IPC_FLAG_FIFO);
	main_event recv_buff;
	while (1) {
		rt_mq_recv(main_events, (void *)&recv_buff, sizeof(main_event), RT_WAITING_FOREVER);
		switch (recv_buff.kind) {
		case SEND_BUTTON:
			action_publish(MQTT_TOPIC_BUTTON_PRESS, recv_buff.payload);
			break;
		case REPORT_RELAY:
			action_publish_retained(MQTT_TOPIC_RELAY_STATE, recv_buff.payload);
			break;
		}
		if (recv_buff.free_payload)
			free((void *)recv_buff.payload);
	}
}
