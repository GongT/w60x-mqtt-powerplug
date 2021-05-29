#define DBG_SECTION_NAME "mqtt"

#include "app.h"
#include "init.h"
#include "paho_mqtt.h"

#include <string.h>

static MQTTClient client;
static int is_started = 0;

void start_mqtt()
{
	if (is_started)
	{
		LOG_I("MQTT is already running");
		return;
	}

	LOG_I("Start client...");
	if (paho_mqtt_start(&client) == 0)
	{
		LOG_I("client start complete!");
		is_started = 1;
	}
	else
	{
		FATAL_ERROR("MQTT client failed start!");
	}
}

void mq_publish(const char *send_str)
{
	MQTTMessage message;
	message.qos = QOS1;
	message.retained = 0;
	message.payload = (void *)send_str;
	message.payloadlen = strlen(send_str);

	MQTTPublish(&client, client.condata.will.topicName.cstring, &message);

	return;
}

static void mqtt_sub_callback(MQTTClient *c, MessageData *msg_data)
{
	*((char *)msg_data->message->payload + msg_data->message->payloadlen) = '\0';
	LOG_D("Topic: %.*s receive a message: %.*s",
		  msg_data->topicName->lenstring.len,
		  msg_data->topicName->lenstring.data,
		  msg_data->message->payloadlen,
		  (char *)msg_data->message->payload);

	return;
}

static void mqtt_sub_default_callback(MQTTClient *c, MessageData *msg_data)
{
	*((char *)msg_data->message->payload + msg_data->message->payloadlen) = '\0';
	LOG_D("mqtt sub default callback: %.*s %.*s",
		  msg_data->topicName->lenstring.len,
		  msg_data->topicName->lenstring.data,
		  msg_data->message->payloadlen,
		  (char *)msg_data->message->payload);
	return;
}

static void mqtt_connect_callback(MQTTClient *c)
{
	LOG_I("Start to connect mqtt server");
}

static void mqtt_online_callback(MQTTClient *c)
{
	LOG_I("Connect mqtt server success");
	mq_publish("appear");
}

static void mqtt_offline_callback(MQTTClient *c)
{
	LOG_I("Disconnect from mqtt server");
}

int mqtt_client_init()
{
	size_t device_id_size = get_storage_size(STORE_KEY_MQTT_DEVID);
	size_t server_uri_size = get_storage_size(STORE_KEY_MQTT_SERVER_URI);
	size_t username_size = get_storage_size(STORE_KEY_MQTT_USER);
	size_t password_size = get_storage_size(STORE_KEY_MQTT_PASS);

	if (device_id_size == 0 || server_uri_size == 0 || username_size == 0 || password_size == 0)
	{
		return 1;
	}

	printf("X %d\n", device_id_size + 2 + sizeof(APPLICATION_KIND) + 1);
	char *sup_pub_topic = malloc(device_id_size + 2 + sizeof(APPLICATION_KIND) + 1);
	char *device_id = malloc(device_id_size);
	char *server_uri = malloc(server_uri_size);
	char *username = malloc(username_size);
	char *password = malloc(password_size);

	printf("Y %x\n", (uint)sup_pub_topic);
	assert(sup_pub_topic != NULL);
	assert(device_id != NULL);
	assert(server_uri != NULL);
	assert(username != NULL);
	assert(password != NULL);

	client.isconnected = 0;
	client.uri = server_uri;

	rt_snprintf(sup_pub_topic, sizeof(sup_pub_topic), "/" APPLICATION_KIND "/%s", device_id);
	LOG_I("MQTT endpoint is: %s", sup_pub_topic);

	/* 配置连接参数 */
	MQTTPacket_connectData condata = MQTTPacket_connectData_initializer;
	memcpy(&client.condata, &condata, sizeof(condata));
	client.condata.clientID.cstring = device_id;
	client.condata.keepAliveInterval = 30;
	client.condata.cleansession = 1;
	client.condata.username.cstring = username;
	client.condata.password.cstring = password;
	client.condata.willFlag = 0;
	client.condata.will.qos = QOS1;
	client.condata.will.retained = 0;
	client.condata.will.topicName.cstring = sup_pub_topic;
	client.buf_size = client.readbuf_size = 1024;
	client.buf = malloc(client.buf_size);
	assert(client.buf != NULL);
	client.readbuf = malloc(client.readbuf_size);
	assert(client.readbuf != NULL);

	/* 设置事件回调 */
	client.connect_callback = mqtt_connect_callback;
	client.online_callback = mqtt_online_callback;
	client.offline_callback = mqtt_offline_callback;
	client.defaultMessageHandler = mqtt_sub_default_callback;

	return 0;
}
