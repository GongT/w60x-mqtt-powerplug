#include "app.h"
#include "init.h"
#include <gongt/config_tool.h>

void config_mode_status_callback(enum CONFIG_STATUS status)
{
	static int last_state = 0;
	switch (status)
	{
	case CONFIG_STATUS_WIFI_CONNECT:
		led_blink(LED_GREEN, 400);
		rt_thread_mdelay(200);
		led_blink(LED_RED, 400);
		break;
	case CONFIG_STATUS_HTTP_SEND:
	case CONFIG_STATUS_FLASH_WRITE:
		last_state = !last_state;
		if (last_state)
			led_on(LED_GREEN);
		else
			led_off(LED_GREEN);
		break;
	case CONFIG_STATUS_OTA_NEW:
		led_on(LED_RED);
		break;
	case CONFIG_STATUS_OTA_END:
	case CONFIG_STATUS_CONFIG_END:
		led_off(LED_RED);
		led_on(LED_GREEN);
		break;
	default:
		break;
	}
}

void alert_config_start()
{
	buzzer_beep(800, 20, 30);
	buzzer_beep(800, 0, 30);
	buzzer_beep(800, 20, 30);
	buzzer_beep(800, 0, 30);
	buzzer_beep(800, 20, 30);
}

void alert_config_end()
{
	led_off(LED_RED);
	led_on(LED_GREEN);
	buzzer_beep(3000, 50, 300);
	buzzer_beep(3000, 0, 30);
	buzzer_beep(1000, 20, 100);
}
void alert_config_fail()
{
	led_off(LED_GREEN);
	led_on(LED_RED);
	buzzer_beep(5000, 30, 1000);
}

static rt_bool_t first_success = RT_TRUE;

static void handle_wifi_connect(int event, struct rt_wlan_buff *buff, void *parameter)
{
	// struct ip_addr_t *ip = (struct ip_addr_t *)buff;
	// rt_kprintf(" * IP Addr: %s", ip->);
	buzzer_beep(523, 30, 50);
	buzzer_beep(3000, 0, 50);
	buzzer_beep(587, 30, 50);
	buzzer_beep(3000, 0, 50);
	buzzer_beep(698, 30, 50);
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

void init_wifi_alert()
{
	rt_wlan_register_event_handler(RT_WLAN_EVT_READY, handle_wifi_connect, RT_NULL);
	rt_wlan_register_event_handler(RT_WLAN_EVT_STA_CONNECTED_FAIL, handle_wifi_error, RT_NULL);
	rt_wlan_register_event_handler(RT_WLAN_EVT_STA_DISCONNECTED, handle_wifi_error, RT_NULL);
}
