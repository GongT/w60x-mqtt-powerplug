#define DBG_SECTION_NAME "init"

#include "app.h"
#include <easyflash.h>
#include <fal.h>
#include <stdlib.h>
#include <gongt/config_tool.h>

#include "init.h"

static void handle_wifi_connect(int event, struct rt_wlan_buff *buff, void *parameter)
{
	struct ip_addr_t *ip = (struct ip_addr_t *)buff;
	LOG_I(" * IP Addr: %s", ip);
}

static void handle_wifi_error(int event, struct rt_wlan_buff *buff, void *parameter)
{
	if (event == RT_WLAN_EVT_STA_CONNECTED_FAIL)
	{
		LOG_E("event: RT_WLAN_EVT_STA_CONNECTED_FAIL");
	}
	else if (event == RT_WLAN_EVT_STA_DISCONNECTED)
	{
		LOG_E("event: RT_WLAN_EVT_STA_DISCONNECTED");
	}
}

static int goto_config_mode_or_return()
{
#if AUTO_GOTO_CONFIG
	goto_config_mode();
	return 1;
#else
	LOG_E("development mode, skip goto_config_mode()");
	return 0;
#endif
}

static int _app_main_init()
{
	if (fal_init() == 0)
	{
		LOG_E("No partitions found!");
		return 1;
	}

#if !FAL_DEBUG
	fal_show_part_table();
#endif

	if (easyflash_init() != EF_NO_ERR)
	{
		LOG_E("Failed to init easyflash!");
		return 1;
	}

	test_env();

	if (mqtt_client_init() != 0)
	{
		return goto_config_mode_or_return();
	}

	rt_wlan_register_event_handler(RT_WLAN_EVT_READY, handle_wifi_connect, RT_NULL);
	rt_wlan_register_event_handler(RT_WLAN_EVT_STA_CONNECTED_FAIL, handle_wifi_error, RT_NULL);
	rt_wlan_register_event_handler(RT_WLAN_EVT_STA_DISCONNECTED, handle_wifi_error, RT_NULL);

	if (connect_wifi() != 0)
	{
		return goto_config_mode_or_return();
	}
	start_mqtt();

	return 0;
}

static int app_main_init()
{
	if (_app_main_init() != 0)
	{
		rt_enter_critical();
		LOG_E("\rreboot in 5s...");
		ulog_flush();
		rt_thread_mdelay(5000);
#if DISABLE_REBOOT
		LOG_E("\rreboot is disabled.");
		ulog_flush();
#else
		LOG_E("\rreboot...");
		ulog_flush();
		rt_hw_cpu_reset();
#endif
		while (1)
			;
	}
	return 0;
}
INIT_APP_EXPORT(app_main_init);
