#include "app.h"
#include "init.h"
#include <easyflash.h>
#include <sys/socket.h>
#include <netdev.h>
#include <rthw.h>
#include <gongt/config_tool.h>
extern void print_netdev_list();
void print_netdev_list() {
	rt_base_t level;
	rt_slist_t *node = RT_NULL;
	struct netdev *netdev = RT_NULL;

	if (netdev_list == RT_NULL) {
		KPRINTF_COLOR(9, " * netdev_list is empty");
		return;
	}

	level = rt_hw_interrupt_disable();

	KPRINTF_COLOR(10, "netdev_list:");
	for (node = &(netdev_list->list); node; node = rt_slist_next(node)) {
		netdev = rt_slist_entry(node, struct netdev, list);
		rt_kprintf(" * %.*s\n", RT_NAME_MAX, netdev->name);
	}

	rt_hw_interrupt_enable(level);

	return;
}

int connect_wifi() {
	size_t wifi_name_size = get_storage_size(STORE_KEY_WIFI_NAME);
	size_t wifi_pass_size = get_storage_size(STORE_KEY_WIFI_PASS);

	if (!wifi_name_size || !wifi_pass_size) {
		KPRINTF_COLOR(5, "no wifi name and password.");
		return 1;
	}

	char wifi_name[wifi_name_size + 1];
	memset(wifi_name, 0, wifi_name_size + 1);
	char wifi_pass[wifi_pass_size + 1];
	memset(wifi_pass, 0, wifi_pass_size + 1);

	ef_get_env_blob(STORE_KEY_WIFI_NAME, wifi_name, wifi_name_size, NULL);
	ef_get_env_blob(STORE_KEY_WIFI_PASS, wifi_pass, wifi_pass_size, NULL);
	if (strnlen(wifi_name, wifi_name_size) == 0 || strnlen(wifi_pass, wifi_pass_size) == 0) {
		KPRINTF_COLOR(9, "wifi name or pass empty.");
		return 1;
	}

	// print_netdev_list();

	if (rt_wlan_set_mode(RT_WLAN_DEVICE_STA_NAME, RT_WLAN_STATION) != RT_EOK)
		PHYSICAL_ERROR("failed set wlan mode!");
	KPRINTF_DIM("set wlan mode success.");

	struct netdev *dev = CONFIG_INTERFACE_NETDEV;
	if (dev == NULL)
		PHYSICAL_ERROR("can not find netdev!");

	assert(netdev_dhcp_enabled(dev, RT_TRUE) == 0);

	KPRINTF_COLOR(14, "Connect WiFi: %.*s | %.*s", wifi_name_size, wifi_name, wifi_pass_size, wifi_pass);

	while (1) {
	RECONNECT:
		KPRINTF_DIM("connect...");
		if (rt_wlan_connect(wifi_name, wifi_pass) != RT_EOK) {
			KPRINTF_COLOR(11, "wifi connect fail.");
			rt_thread_mdelay(5000);
			continue;
		}

		KPRINTF_COLOR(10, "WiFi connected.");
		while (!rt_wlan_is_ready()) {
			rt_thread_mdelay(1000);
			if (!rt_wlan_is_connected()) {
				KPRINTF_COLOR(9, "WiFi connection broken.");
				goto RECONNECT;
			}
		}

		KPRINTF_COLOR(10, "WiFi is ready.");
		break;
	}

	wifi_status_dump();

	return 0;
}
