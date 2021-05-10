#define DBG_SECTION_NAME "gpio"

#include "io.h"

#define PWM_DEV_NAME "pwm"

struct rt_device_pwm *pwm_dev;

static int gpio_init()
{
	pwm_dev = (struct rt_device_pwm *)rt_device_find(PWM_DEV_NAME);
	if (pwm_dev == RT_NULL)
		PHYSICAL_ERROR("pwm sample run failed! can't find %s device!", PWM_DEV_NAME);

	beep_thread_init();
	led_thread_init();
	key_press_init();
	relay_init();
	return 0;
}
INIT_COMPONENT_EXPORT(gpio_init);
