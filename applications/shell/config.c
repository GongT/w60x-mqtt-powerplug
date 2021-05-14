#define DBG_SECTION_NAME "sh:config"

#include "shell.h"
#include <gongt/config_tool.h>

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
	return goto_config_mode_with_alert();
}

MSH_CMD_EXPORT_ALIAS(goto_config_mode_shell, config, start WiFi config mode);
