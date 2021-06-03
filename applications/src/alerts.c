#include "app.h"
#include <gongt/config_tool.h>

void alert_config_start()
{
	led_fade(LED_GREEN, 2000);
	buzzer_beep(800, 100, 30);
	buzzer_beep(800, 0, 30);
	buzzer_beep(800, 100, 30);
	buzzer_beep(800, 0, 30);
	buzzer_beep(800, 100, 30);
}

void alert_config_end()
{
	led_off(LED_RED);
	led_on(LED_GREEN);
	buzzer_beep(3000, 70, 50);
	buzzer_beep(3000, 0, 50);
	buzzer_beep(1000, 40, 50);
}
void alert_config_fail()
{
	led_off(LED_GREEN);
	led_on(LED_RED);
	buzzer_beep(3000, 30, 600);
}

static rt_bool_t first_success = RT_TRUE;

static void handle_wifi_connect(int event, struct rt_wlan_buff *buff, void *parameter)
{
	// struct ip_addr_t *ip = (struct ip_addr_t *)buff;
	// rt_kprintf(" * IP Addr: %s", ip->);

	if (first_success)
	{
		first_success = RT_FALSE;
	}
	else
	{
		buzzer_beep(523, 30, 50);
		buzzer_beep(3000, 0, 50);
		buzzer_beep(587, 30, 50);
		buzzer_beep(3000, 0, 50);
		buzzer_beep(698, 30, 50);
	}
}

static void handle_wifi_error(int event, struct rt_wlan_buff *buff, void *parameter)
{
	first_success = RT_FALSE;

	buzzer_beep(698, 30, 50);
	buzzer_beep(3000, 0, 50);
	buzzer_beep(587, 30, 50);
	buzzer_beep(3000, 0, 50);
	buzzer_beep(523, 30, 50);

	if (event == RT_WLAN_EVT_STA_CONNECTED_FAIL)
	{
		KPRINTF_COLOR(9, "event: RT_WLAN_EVT_STA_CONNECTED_FAIL");
	}
	else if (event == RT_WLAN_EVT_STA_DISCONNECTED)
	{
		KPRINTF_COLOR(9, "event: RT_WLAN_EVT_STA_DISCONNECTED");
	}
}

static int init_wifi_alert()
{
	rt_wlan_register_event_handler(RT_WLAN_EVT_READY, handle_wifi_connect, RT_NULL);
	rt_wlan_register_event_handler(RT_WLAN_EVT_STA_CONNECTED_FAIL, handle_wifi_error, RT_NULL);
	rt_wlan_register_event_handler(RT_WLAN_EVT_STA_DISCONNECTED, handle_wifi_error, RT_NULL);

	return 0;
}
INIT_COMPONENT_EXPORT(init_wifi_alert);
