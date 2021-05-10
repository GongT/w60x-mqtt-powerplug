#ifdef RT_USING_FINSH

#define DBG_SECTION_NAME "ELE:TEST"
#include "app.h"

#include <finsh.h>
#define RETURN_LOG_E(...)   \
	{                       \
		LOG_E(__VA_ARGS__); \
		return 1;           \
	}

extern struct rt_device_pwm *pwm_dev;

static long key_press_status_msh(int argc, char **argv)
{
	LOG_I("Key Value: %d", rt_pin_read(PIN_KEY));
}
MSH_CMD_EXPORT_ALIAS(key_press_status_msh, get_key, get_key_status);

static long set_led_msh(int argc, char **argv)
{
	if (argc != 3)
		RETURN_LOG_E("led 1/2 [0-100]");

	int status = atoi(argv[2]);
	if (status < 0 || status > 100)
		RETURN_LOG_E("led 1/2 [0-100]");

	if (str_eq(argv[1], "1"))
		red_led(status);
	else if (str_eq(argv[1], "1"))
		green_led(status);
	else
		RETURN_LOG_E("led 1/2 [0-100]");
}
MSH_CMD_EXPORT_ALIAS(set_led_msh, led, set led light);

static long set_relay_msh(int argc, char **argv)
{
	if (str_eq(argv[1], "on"))
		relay_set(1);
	else if (str_eq(argv[1], "off"))
		relay_set(0);
	else
		RETURN_LOG_E("led 1/2 [0-100]");
}
MSH_CMD_EXPORT_ALIAS(set_relay_msh, relay, set relay on / off);

static long test_beep()
{
	buzzer_beep_a(200, 40, 200);
	buzzer_beep_a(400, 40, 200);
	buzzer_beep_a(800, 40, 200);
}
MSH_CMD_EXPORT_ALIAS(test_beep, test_beep, test buzzer working);
#endif //RT_USING_FINSH
