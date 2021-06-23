#include "logic.h"

void mqtt_topic_handler_upgrade(void *client, message_data_t *msg)
{
	KPRINTF_DIM("-----------------------------------------------------------------------------------");
	KPRINTF_DIM("%s:%d %s()...\ntopic: %s\nmessage:%s", __FILE__, __LINE__, __FUNCTION__, msg->topic_name, (char *)msg->message->payload);
	KPRINTF_DIM("-----------------------------------------------------------------------------------");
	set_update_mode();
	rt_thread_mdelay(100);
	rt_hw_cpu_reset();
}
