#define DBG_SECTION_NAME "sh:config"

#include "shell.h"
#include <gongt/config_tool.h>

static long goto_config_mode_with_alert_main(int argc, char **argv)
{
	return goto_config_mode_with_alert();
}

static int goto_config_mode_shell(void)
{
	if (rt_wlan_is_connected())
	{
		LOG_I("WiFi disconnect...");
		if (rt_wlan_disconnect() != RT_EOK)
		{
			LOG_E("failed disconnect WiFi.");
			return 1;
		}
	}
	rt_thread_startup(rt_thread_create("cfg", goto_config_mode_with_alert_main, NULL, 4096, RT_THREAD_PRIORITY_MAX / 2, 10));
	return 0;
}
MSH_CMD_EXPORT_ALIAS(goto_config_mode_shell, config, start WiFi config mode);
