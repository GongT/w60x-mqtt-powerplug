#include "shell.h"
#include <gongt/config_tool.h>

static void goto_config_mode_with_alert_main(void *args)
{
	goto_config_mode_with_alert();
}

static long goto_config_mode_shell(int argc, char **argv)
{
	if (rt_wlan_is_connected())
	{
		KPRINTF_DIM("WiFi disconnect...");
		if (rt_wlan_disconnect() != RT_EOK)
		{
			KPRINTF_COLOR(9, "failed disconnect WiFi.");
			return 1;
		}
	}
	rt_thread_startup(rt_thread_create("cfg", goto_config_mode_with_alert_main, NULL, 4096, RT_THREAD_PRIORITY_MAX / 2, 10));
	return 0;
}
DEFINE_CMD(goto_config_mode_shell, config, start WiFi config mode);

static long goto_update_mode_shell(int argc, char **argv)
{
	set_update_mode();
	rt_thread_mdelay(100);
	rt_hw_cpu_reset();
	return 0;
}
DEFINE_CMD(goto_update_mode_shell, update, start WiFi update mode);
