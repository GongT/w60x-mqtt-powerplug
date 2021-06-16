#include "app.h"
#include <easyflash.h>
#ifdef PKG_USING_FAL
#include <fal.h>
#endif
#include <stdlib.h>
#include "init.h"
#include <gongt/config_tool.h>

#ifdef PKG_USING_MBEDTLS
#include <mbedtls/ssl.h>
#endif

rt_mq_t main_events = NULL;
extern time_t ntp_sync_to_rtc(const char *host_name);
static rt_thread_t real_main_thread = NULL;

static void switch_priority()
{
	uint8_t p = 25;
	rt_thread_control(rt_thread_self(), RT_THREAD_CTRL_CHANGE_PRIORITY, &p);
}

static void app_main()
{
	KPRINTF_COLOR(6, "connect_wifi()");
	if (connect_wifi() != 0)
		goto_config_mode_and_quit();

	time_t t = time(NULL);
	while (t == 0)
	{
		KPRINTF_COLOR(6, "ntp_sync_to_rtc()");
		t = ntp_sync_to_rtc(NULL);
		KPRINTF_COLOR(6, "  -> %ld\n", t);
		rt_thread_mdelay(1000);
	}

#ifdef PKG_USING_MBEDTLS
	mbedtls_debug_set_threshold(4);
#endif

	KPRINTF_COLOR(6, "start_mqtt()");
	if (start_mqtt() != 0)
		FATAL_ERROR("failed start mqtt");

	main_events = rt_mq_create("main", sizeof(main_event), 5, RT_IPC_FLAG_FIFO);
	main_event recv_buff;
	while (1)
	{
		rt_mq_recv(main_events, (void *)&recv_buff, sizeof(main_event), RT_WAITING_FOREVER);
		switch (recv_buff.kind)
		{
		case SEND_BUTTON:
			action_publish(MQTT_TOPIC_BUTTON_PRESS, recv_buff.payload);
			break;
		case REPORT_RELAY:
			action_publish_retained(MQTT_TOPIC_RELAY_STATE, recv_buff.payload);
			break;
		case SET_LED:
			break;
		case SET_BEEP:
			break;
		case SET_RELAY:
if(strcmp(recv_buff.)
			break;
		}
		if (recv_buff.free_payload)
			free((void *)recv_buff.payload);
	}
	// KPRINTF_COLOR(11, "app main thread done.");
}

int main(void)
{
	print_internal_flash_map();

	KPRINTF_COLOR(6, "Main function has called...");
	switch_priority();

#ifdef PKG_USING_FAL
	if (fal_init() == 0)
		PHYSICAL_ERROR("No partitions found!");
#if defined(FAL_DEBUG) && !FAL_DEBUG
	fal_show_part_table();
#endif
#endif

	KPRINTF_COLOR(6, "easyflash_init()");
	if (easyflash_init() != EF_NO_ERR)
		PHYSICAL_ERROR("Failed to init easyflash!");

	if (0)
	{
		KPRINTF_COLOR(6, "test_env()");
		test_env();
		size_t bt_size = get_storage_size("boot_times");
		char bt[bt_size];
		ef_get_env_blob("boot_times", bt, bt_size, NULL);
		rt_kprintf("Boot Times (size=%d): %.*s\n", bt_size, bt_size, bt);
	}

	if (key_is_pressed())
	{
		KPRINTF_COLOR(14, "release in 1s exit main.");
		buzzer_beep(1000, 5, 10);
		rt_thread_mdelay(1000);

		if (key_is_pressed())
		{
			goto_config_mode_and_quit();
			return 0;
		}
		else
		{
			buzzer_beep(200, 5, 500);
			return 0;
		}
	}

	real_main_thread = rt_thread_create("user-app", app_main, NULL, 10240, 20, 20);
	rt_thread_startup(real_main_thread);

	return 0;
}
