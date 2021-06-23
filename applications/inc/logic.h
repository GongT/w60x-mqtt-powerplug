#pragma once

#include "app.h"
#include <mqttclient.h>

void mqtt_topic_handler_beep(void *client, message_data_t *msg);
void mqtt_topic_handler_relay(void *client, message_data_t *msg);
void mqtt_topic_handler_led(void *client, message_data_t *msg);
void mqtt_topic_handler_upgrade(void *client, message_data_t *msg);
