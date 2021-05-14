#define DBG_SECTION_NAME "cfg.beep"

#include "app.h"
#include <gongt/config_tool.h>

void config_mode_status_hook(enum CONFIG_STATUS why)
{
	LOG_I("config mode status: %d", why);
	if (why == CONFIG_STATUS_INIT)
	{
		led_fade(LED_GREEN, 2000);
		buzzer_beep(800, 100, 30);
		buzzer_beep(800, 0, 100);
		buzzer_beep(800, 100, 30);
		buzzer_beep(800, 0, 100);
		buzzer_beep(800, 100, 30);
	}
	else
	{
		led_off(LED_RED);
		led_on(LED_GREEN);
		buzzer_beep(3000, 60, 1000);
		buzzer_beep(3000, 0, 1000);
		buzzer_beep(3000, 60, 1000);
		buzzer_beep(3000, 0, 1000);
		buzzer_beep(3000, 60, 1000);
	}
}
