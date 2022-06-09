#include "logic.h"

void mqtt_topic_handler_relay(void *client, message_data_t *msg) {
	KPRINTF_DIM("-----------------------------------------------------------------------------------");
	KPRINTF_DIM("%s:%d %s()...\ntopic: %s\nmessage:%s", __FILE__, __LINE__, __FUNCTION__, msg->topic_name, (char *)msg->message->payload);
	KPRINTF_DIM("-----------------------------------------------------------------------------------");

	if (strncmp(msg->message->payload, "0", 1) == 0) {
		KPRINTF_COLOR(10, "MQTT recv: set relay OFF");
		relay_set(RELAY_OFF);
	} else if (strncmp(msg->message->payload, "1", 1) == 0) {
		KPRINTF_COLOR(10, "MQTT recv: set relay ON");
		relay_set(RELAY_ON);
	} else {
		KPRINTF_COLOR(11, "MQTT recv: set relay [%.*s] but want 1/0.", msg->message->payloadlen, msg->message->payload);
	}
}
