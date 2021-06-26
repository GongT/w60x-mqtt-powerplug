#pragma once

#include "app.h"

extern struct rt_device_pwm *pwm_dev;

void beep_thread_init();
// extern struct rt_mutex mutex_beep;
// extern struct rt_mutex mutex_led;
// extern struct rt_mutex mutex_relay;
void key_press_init();
void led_thread_init();
void relay_init();

#ifdef DEBUG_DEVICE
#define _DEV_DEBUG(...) KPRINTF_DIM(__VA_ARGS__)
#else
#define _DEV_DEBUG(...)
#endif
