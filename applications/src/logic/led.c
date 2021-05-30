#define DBG_SECTION_NAME "l:led"

#include "logic.h"

void mqtt_topic_handler_led(void *client, message_data_t *msg)
{
	LOG_I("-----------------------------------------------------------------------------------");
	LOG_I("%s:%d %s()...\ntopic: %s\nmessage:%s", __FILE__, __LINE__, __FUNCTION__, msg->topic_name, (char *)msg->message->payload);
	LOG_I("-----------------------------------------------------------------------------------");
}
