#define DBG_SECTION_NAME "gpio.led"

#include "io.h"

static const uint16_t LIGHT_LEVELS[] = {
	0};

inline const char *title(enum led_id id)
{
	if (id == PWM_CH_RED)
		return "<red led>";
	else if (id == PWM_CH_GREEN)
		return "<green led>";
	else
		return "<invalid led>";
}

void led_blink(enum led_id id, uint32_t on_time_ms, uint32_t off_time_ms)
{
}

void led_fade(enum led_id id, uint32_t on_time_ms, uint32_t off_time_ms)
{
}

void led_static(enum led_id id, uint32_t light_percent)
{
	LOG_I("%s static level: %d", title(id), light_percent);
	assert0(rt_pwm_set(pwm_dev, id, 20000, (20000 / 100) * light_percent));
}

void led_off(enum led_id id)
{
	led_static(id, 0);
}

void led_on(enum led_id id)
{
	led_static(id, 100);
}

void led_thread_init()
{
	assert0(rt_pwm_enable(pwm_dev, PWM_CH_RED));
	assert0(rt_pwm_enable(pwm_dev, PWM_CH_GREEN));
}
