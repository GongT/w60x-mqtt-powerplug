#include "io.h"

#define PWM_DEV_NAME "pwm"

struct rt_device_pwm *pwm_dev;

static int gpio_init() {
	KPRINTF_DIM("Start...");

	pwm_dev = (struct rt_device_pwm *)rt_device_find(PWM_DEV_NAME);
	if (pwm_dev == RT_NULL)
		PHYSICAL_ERROR("pwm sample run failed! can't find %s device!", PWM_DEV_NAME);

	KPRINTF_DIM(" * beep.");
	beep_thread_init();
	KPRINTF_DIM(" * led.");
	led_thread_init();
	KPRINTF_DIM(" * key0.");
	key_press_init();
	KPRINTF_DIM(" * relay.");
	relay_init();
	KPRINTF_DIM("Done.");

	return 0;
}
INIT_COMPONENT_EXPORT(gpio_init);
