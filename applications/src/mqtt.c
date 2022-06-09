#include <string.h>
#include <stdio.h>
#include <stdint.h>

#include "app.h"
#include "init.h"
#include "logic.h"

#include <easyflash.h>
#include <mqttclient.h>
#ifdef KAWAII_MQTT_NETWORK_TYPE_TLS
// #include <tls_certificate.h>
#endif // KAWAII_MQTT_NETWORK_TYPE_TLS

rt_bool_t mqtt_init_complete = RT_FALSE;

mqtt_client_t *client = NULL;
#define DEVICE_ROOT_TOPIC "$devices"

const char *mqtt_base_topic_name = NULL;
char mqtt_device_id[13];
const char *mqtt_server_hostname = NULL;
const char *mqtt_server_port = NULL;
const char *mqtt_username = NULL;
const char *mqtt_password = NULL;

const char *mqtt_input_topic_beep;
const char *mqtt_input_topic_relay;
const char *mqtt_input_topic_led;
const char *mqtt_input_topic_update;
const char *mqtt_output_topic_button;
const char *mqtt_output_topic_relay;
const char *mqtt_output_topic_keepalive;

static char *create_topic(const char *name) {
	size_t size = strlen(mqtt_base_topic_name) + strlen(name) + 1;
	char *buff = malloc(size);
	assert(buff != NULL);
	snprintf(buff, size, "%s%s", mqtt_base_topic_name, name);
	return buff;
}

#define create_arg_safe(VAR, KN) \
	VAR = create_arg(KN);        \
	if (VAR == NULL)             \
		return RT_EINVAL;

static char *create_arg(const char *KEY_NAME) {
	size_t param_size = get_storage_size(KEY_NAME) + 1;
	if (param_size == 0) {
		KPRINTF_COLOR(9, "%s size is 0", KEY_NAME);
		return NULL;
	}
	char *param_stor = malloc(param_size);
	assert(param_stor != NULL);
	memset(param_stor, 0, param_size);
	ef_get_env_blob(KEY_NAME, param_stor, param_size, NULL);
	return param_stor;
}

static rt_err_t mqtt_params_init() {
	if (mqtt_base_topic_name != NULL)
		return RT_EOK;

	rt_uint8_t mac[6];
	rt_wlan_get_mac(mac);
	sprintf(mqtt_device_id, "%02x%02x%02x%02x%02x%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

	char *device_title;
	create_arg_safe(device_title, STORE_KEY_MQTT_DEV_TITLE);
	KPRINTF_DIM("MQTT device title is: [%s]", device_title);

	char *server_hostname;
	create_arg_safe(server_hostname, STORE_KEY_MQTT_SERVER_URI);
	mqtt_server_hostname = server_hostname;
	char *port = strchr(server_hostname, ':');
	if (port == NULL)
		mqtt_server_port = "8883";
	else {
		*port = '\0';
		mqtt_server_port = port + 1;
	}
	KPRINTF_DIM("MQTT server is: [%s]:[%s]", mqtt_server_hostname, mqtt_server_port);

	create_arg_safe(mqtt_username, STORE_KEY_MQTT_USER);
	KPRINTF_DIM("MQTT username is: [%s]", mqtt_username);

	create_arg_safe(mqtt_password, STORE_KEY_MQTT_PASS);
	KPRINTF_DIM("MQTT password is: [%s]", mqtt_password);

	const size_t base_topic_name_size = strlen(DEVICE_ROOT_TOPIC) + 1 + strlen(APPLICATION_KIND) + 1 + strlen(device_title) + 2;
	char *base_topic_name = malloc(base_topic_name_size);
	mqtt_base_topic_name = base_topic_name;
	assert(base_topic_name != NULL);

	rt_snprintf(base_topic_name, base_topic_name_size, DEVICE_ROOT_TOPIC "/" APPLICATION_KIND "/%s/", device_title);
	KPRINTF_DIM("MQTT endpoint is: [%s]", base_topic_name);

	mqtt_input_topic_beep = create_topic("beep");
	mqtt_input_topic_relay = create_topic("relay");
	mqtt_input_topic_led = create_topic("led");
	mqtt_output_topic_relay = create_topic("$relay");
	mqtt_output_topic_keepalive = create_topic("alive");
	mqtt_output_topic_button = create_topic("button");

	free(device_title);

	return RT_EOK;
}

int start_mqtt(void) {
	mqtt_log_init();

	if (mqtt_params_init() != RT_EOK)
		return 1;

	client = mqtt_lease();

	mqtt_set_host(client, (char *)mqtt_server_hostname);
	mqtt_set_port(client, (char *)mqtt_server_port);
	mqtt_set_user_name(client, (char *)mqtt_username);
	mqtt_set_password(client, (char *)mqtt_password);
	mqtt_set_client_id(client, (char *)mqtt_device_id);
	mqtt_set_will_flag(client, 1);
	// mqtt_set_cmd_timeout(client, )
	mqtt_set_keep_alive_interval(client, 30);
#ifdef KAWAII_MQTT_NETWORK_TYPE_TLS
	mqtt_set_ca(client, mbedtls_root_certificate);
#endif
	mqtt_set_clean_session(client, 1);

	mqtt_set_will_options(client, mqtt_output_topic_keepalive, QOS0, 0, "");

	int ret = mqtt_connect(client);
	if (ret != KAWAII_MQTT_SUCCESS_ERROR) {
		FATAL_ERROR("mqtt connection failed: %d", ret);
		return ret;
	}

	rt_thread_mdelay(1000);

	mqtt_subscribe(client, mqtt_input_topic_relay, QOS1, mqtt_topic_handler_relay);
	mqtt_subscribe(client, mqtt_input_topic_beep, QOS0, mqtt_topic_handler_beep);
	mqtt_subscribe(client, mqtt_input_topic_led, QOS0, mqtt_topic_handler_led);

	mqtt_subscribe(client, mqtt_input_topic_update, QOS0, mqtt_topic_handler_upgrade);

	mqtt_init_complete = RT_TRUE;

	return 0;
}

rt_bool_t is_mqtt_init() {
	return mqtt_init_complete;
}

static int _action_publish(enum mqtt_topic topic, const char *send_data, uint8_t retained) {
	mqtt_message_t msg;
	memset(&msg, 0, sizeof(msg));

	msg.qos = 1;
	msg.payload = (void *)send_data;
	msg.payloadlen = strlen(send_data);
	msg.retained = retained;

	const char *topic_str;
	switch (topic) {
	case MQTT_TOPIC_BUTTON_PRESS:
		topic_str = mqtt_output_topic_button;
		break;
	case MQTT_TOPIC_RELAY_STATE:
		topic_str = mqtt_output_topic_relay;
		break;
	case MQTT_TOPIC_ALIVE:
		topic_str = mqtt_output_topic_keepalive;
		break;
	default:
		KPRINTF_COLOR(9, "action_publish() missing some switch branch!");
		return -1;
	}

	int r = mqtt_publish(client, topic_str, &msg);

	if (r == 0)
		KPRINTF_DIM("publish topic: %s = %s | ok", topic_str, send_data);
	else
		KPRINTF_COLOR(11, "publish topic: %s = %s | return %d", topic_str, send_data, r);

	return r;
}

int action_publish(enum mqtt_topic topic, const char *send_data) {
	return _action_publish(topic, send_data, 0);
}
int action_publish_retained(enum mqtt_topic topic, const char *send_data) {
	return _action_publish(topic, send_data, 1);
}
