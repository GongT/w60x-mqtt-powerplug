#define DBG_SECTION_NAME "wifi"

#include "app.h"
#include "init.h"
#include <easyflash.h>

void wifi_status_dump();

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

static init_wifi()
{
	rt_wlan_register_event_handler(RT_WLAN_EVT_READY, handle_wifi_connect, RT_NULL);
	rt_wlan_register_event_handler(RT_WLAN_EVT_STA_CONNECTED_FAIL, handle_wifi_error, RT_NULL);
	rt_wlan_register_event_handler(RT_WLAN_EVT_STA_DISCONNECTED, handle_wifi_error, RT_NULL);
}

int connect_wifi()
{
	rt_wlan_set_mode(RT_WLAN_DEVICE_STA_NAME, RT_WLAN_STATION);

	size_t wifi_name_size = get_storage_size(STORE_KEY_WIFI_NAME);
	size_t wifi_pass_size = get_storage_size(STORE_KEY_WIFI_PASS);

	if (!wifi_name_size || !wifi_pass_size)
	{
		LOG_I("no wifi name and password.");
		return 1;
	}

	char wifi_name[wifi_name_size + 1];
	char wifi_pass[wifi_pass_size + 1];

	ef_get_env_blob(STORE_KEY_WIFI_NAME, wifi_name, wifi_name_size, NULL);
	ef_get_env_blob(STORE_KEY_WIFI_PASS, wifi_pass, wifi_pass_size, NULL);

	while (1)
	{
		LOG_I("Connect WiFi: %s | %s", wifi_name, wifi_pass);
		if (rt_wlan_connect(wifi_name, wifi_pass) != RT_EOK)
		{
			LOG_W("wifi connect fail.");
			continue;
		}

		LOG_I("WiFi connected.");
		while (!rt_wlan_is_ready())
		{
			rt_thread_mdelay(500);
			if (!rt_wlan_is_connected())
			{
				LOG_W("WiFi connection broken.");
				continue;
			}
		}

		LOG_I("WiFi is ready.");
		wifi_status_dump();
	}

	return 0;
}
