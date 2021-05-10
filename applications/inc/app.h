#pragma once

#include "local_configs.h"

#if !defined(DBG_TAG) && !defined(DBG_SECTION_NAME)
#warning "此文件没有定义C语言常量DBG_TAG或DBG_SECTION_NAME，默认为unknown"
#define DBG_TAG "main"
#endif
#ifndef DBG_LVL
#define DBG_LVL DBG_LOG
#endif
#include <rtdbg.h>

#include <rtthread.h>
#include <rtdevice.h>

#include "storage.h"
#include "helpers.h"

size_t get_storage_size(const char *key);
void mq_publish(const char *send_str);

/* 按键 */
rt_bool_t key_is_pressed();

/* 蜂鸣器 */
void beep_error();
void buzzer_beep(uint16_t toneHz, uint8_t volumePercent, uint32_t timeMs);

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
	LED_RED = PWM_CH_RED,
	LED_GREEN = PWM_CH_GREEN,
};
void led_blink(enum led_id id, uint32_t on_time_ms, uint32_t off_time_ms);
void led_fade(enum led_id id, uint32_t on_time_ms, uint32_t off_time_ms);
void led_static(enum led_id id, uint32_t light_percent);
void led_off(enum led_id id);
void led_on(enum led_id id);
