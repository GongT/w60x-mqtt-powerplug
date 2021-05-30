#define DBG_SECTION_NAME "l:relay"

#include "logic.h"

void mqtt_topic_handler_relay(void *client, message_data_t *msg)
{
	LOG_I("-----------------------------------------------------------------------------------");
	LOG_I("%s:%d %s()...\ntopic: %s\nmessage:%s", __FILE__, __LINE__, __FUNCTION__, msg->topic_name, (char *)msg->message->payload);
	LOG_I("-----------------------------------------------------------------------------------");
}
