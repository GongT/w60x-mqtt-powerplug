#define DBG_SECTION_NAME "mqtt"

#include <string.h>
#include <stdio.h>
#include <stdint.h>

#include "app.h"
#include "init.h"
#include "logic.h"

#include <easyflash.h>
#include <mqttclient.h>

mqtt_client_t *client = NULL;
#define DEVICE_ROOT_TOPIC "$devices"
#define assert_not_zero(x) \
	if (x == 0)            \
	{                      \
		LOG_E(#x " is 0"); \
		return 1;          \
	}

const char *mqtt_base_topic_name = NULL;
const char mqtt_device_id[13];
const char *mqtt_server_hostname = NULL;
const char *mqtt_server_port = NULL;
const char *mqtt_username = NULL;
const char *mqtt_password = NULL;

const char *mqtt_input_topic_beep;
const char *mqtt_input_topic_relay;
const char *mqtt_input_topic_led;
const char *mqtt_output_topic_button;

static char *create_topic(const char *name)
{
	size_t size = strlen(mqtt_base_topic_name) + strlen(name) + 1;
	char *buff = malloc(size);
	assert(buff != NULL);
	snprintf(buff, size, "%s%s", mqtt_base_topic_name, name);
	return buff;
}

static int mqtt_params_init()
{
	if (mqtt_base_topic_name != NULL)
		return 0;

	rt_uint8_t mac[6];
	rt_wlan_get_mac(mac);
	sprintf(mqtt_device_id, "%02x%02x%02x%02x%02x%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

	size_t device_title_size = get_storage_size(STORE_KEY_MQTT_DEV_TITLE) + 1;
	assert_not_zero(device_title_size);
	char device_title[device_title_size];
	assert(device_title != NULL);
	ef_get_env_blob(STORE_KEY_MQTT_DEV_TITLE, device_title, device_title_size, NULL);
	LOG_I("MQTT dev title is: %s", device_title);

	size_t server_hostname_size = get_storage_size(STORE_KEY_MQTT_SERVER_URI) + 1;
	assert_not_zero(server_hostname_size);
	char *server_hostname = malloc(server_hostname_size);
	mqtt_server_hostname = server_hostname;
	assert(server_hostname != NULL);
	ef_get_env_blob(STORE_KEY_MQTT_SERVER_URI, server_hostname, server_hostname_size, NULL);
	LOG_I("MQTT server is: %s", server_hostname);

	char *port = strchr(server_hostname, ':');
	if (port == NULL)
		mqtt_server_port = "8883";
	else
	{
		*port = '\0';
		mqtt_server_port = port + 1;
	}

	size_t username_size = get_storage_size(STORE_KEY_MQTT_USER) + 1;
	assert_not_zero(username_size);
	char *username = malloc(username_size);
	mqtt_username = username;
	assert(username != NULL);
	ef_get_env_blob(STORE_KEY_MQTT_USER, username, username_size, NULL);
	LOG_I("MQTT username is: %s", username);

	size_t password_size = get_storage_size(STORE_KEY_MQTT_PASS) + 1;
	assert_not_zero(password_size);
	char *password = malloc(password_size);
	mqtt_password = password;
	assert(password != NULL);
	ef_get_env_blob(STORE_KEY_MQTT_PASS, password, password_size, NULL);
	LOG_I("MQTT password is: %s", password);

	const size_t base_topic_name_size = 1 + strlen(DEVICE_ROOT_TOPIC) + 1 + strlen(APPLICATION_KIND) + 1 + strlen(device_title) + 2;
	char *base_topic_name = malloc(base_topic_name_size);
	mqtt_base_topic_name = base_topic_name;
	assert(base_topic_name != NULL);

	rt_snprintf(base_topic_name, base_topic_name_size, "/" DEVICE_ROOT_TOPIC "/" APPLICATION_KIND "/%s/", device_title);
	LOG_I("MQTT endpoint is: %s", base_topic_name);

	mqtt_input_topic_beep = create_topic("set/beep");
	mqtt_input_topic_relay = create_topic("set/relay");
	mqtt_input_topic_led = create_topic("set/led");
	mqtt_output_topic_button = create_topic("get/button");

	return 0;
}

int start_mqtt(void)
{
	mqtt_log_init();

	if (!mqtt_params_init())
		return 1;

	client = mqtt_lease();

	mqtt_set_host(client, (char *)mqtt_server_hostname);
	mqtt_set_port(client, (char *)mqtt_server_port);
	mqtt_set_user_name(client, (char *)mqtt_username);
	mqtt_set_password(client, (char *)mqtt_password);
	mqtt_set_client_id(client, (char *)mqtt_device_id);
	mqtt_set_will_flag(client, 1);
	mqtt_set_keep_alive_interval(client, 30);

	mqtt_set_will_options(client, create_topic("connected"), QOS1, 0, "");

	mqtt_connect(client);
	mqtt_subscribe(client, mqtt_input_topic_relay, QOS1, mqtt_topic_handler_relay);
	mqtt_subscribe(client, mqtt_input_topic_beep, QOS0, mqtt_topic_handler_beep);
	mqtt_subscribe(client, mqtt_input_topic_led, QOS0, mqtt_topic_handler_led);
}

int action_publish(enum mqtt_topic topic, const char *send_data)
{
	mqtt_message_t msg;
	memset(&msg, 0, sizeof(msg));

	msg.qos = 1;
	msg.payload = (void *)send_data;
	msg.payloadlen = strlen(send_data);

	const char *topic_str;
	switch (topic)
	{
	case MQTT_TOPIC_BUTTON_PRESS:
		topic_str = mqtt_output_topic_button;
		break;
	}

	return mqtt_publish(client, topic_str, &msg);
}
