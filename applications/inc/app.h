#pragma once

#include "local_configs.h"
#include "helpers.h"

#include <rtthread.h>
#include <rtdevice.h>

#include "storage.h"

size_t get_storage_size(const char *key);
void mq_publish(const char *send_str);

/* 按键 */
rt_bool_t key_is_pressed();

/* 蜂鸣器 */
void beep_error();
void buzzer_beep(uint16_t toneHz, uint8_t volumePercent, uint32_t timeMs);

struct beep_state
{
	uint16_t toneHz;
	uint8_t volumePercent;
	uint32_t timeMs;
};
typedef struct beep_state beep_state;
typedef struct beep_state *beep_state_t;

/* 继电器 */
enum relay_state_t
{
	RELAY_OFF = PIN_LOW,
	RELAY_ON = PIN_HIGH,
};

void relay_set(enum relay_state_t on);

/* LED灯 */
enum led_id
{
	LED_RED = 0,
	LED_GREEN = 1,
} __attribute__((__packed__));

void led_blink(enum led_id id, uint32_t cycle_ms);
void led_fade(enum led_id id, uint32_t cycle_ms);
void led_static(enum led_id id, uint8_t light_percent);
void led_off(enum led_id id);
void led_on(enum led_id id);

/* config mode */
void alert_config_start();
void alert_config_end();
void alert_config_fail();
enum CONFIG_STATUS goto_config_mode_with_alert();

/* wifi */
