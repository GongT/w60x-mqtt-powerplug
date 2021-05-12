#define DBG_SECTION_NAME "INIT"

#include "io.h"

#define PWM_DEV_NAME "pwm"

struct rt_device_pwm *pwm_dev;

static int gpio_init()
{
	LOG_I("Start...");

	pwm_dev = (struct rt_device_pwm *)rt_device_find(PWM_DEV_NAME);
	if (pwm_dev == RT_NULL)
		PHYSICAL_ERROR("pwm sample run failed! can't find %s device!", PWM_DEV_NAME);

	LOG_I(" * beep.");
	beep_thread_init();
	LOG_I(" * led.");
	led_thread_init();
	LOG_I(" * key0.");
	key_press_init();
	LOG_I(" * relay.");
	relay_init();
	LOG_I("Done.");

	return 0;
}
INIT_COMPONENT_EXPORT(gpio_init);
