#define DBG_SECTION_NAME "main.config"

#include "app.h"
#include <gongt/config_tool.h>

__attribute__((noreturn)) extern void rt_hw_cpu_reset();

__attribute__((noreturn)) inline static void reboot()
{
	rt_enter_critical();
	rt_kprintf("\n");
	for (int i = 5; i > 0; i--)
	{
		LOG_E("system will reboot in %ds...", i);
		rt_thread_mdelay(1000);
	}

	LOG_E("reboot...");
	rt_hw_cpu_reset();
	while (1)
		;
}

enum CONFIG_STATUS goto_config_mode_with_alert()
{
	alert_config_start();

	enum CONFIG_STATUS ret = goto_config_mode();
	if (ret == CONFIG_STATUS_SUCCESS)
	{
		alert_config_end();
	}
	else
	{
		alert_config_fail();
	}
	return ret;
}
