#include "app.h"
#include <easyflash.h>
#ifdef PKG_USING_FAL
#include <fal.h>
#endif
#include <stdlib.h>
#include "init.h"
#include <gongt/config_tool.h>

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

	KPRINTF_COLOR(6, "start_mqtt()");
	if (start_mqtt() != 0)
		FATAL_ERROR("failed start mqtt");

	KPRINTF_COLOR(11, "app main thread done.");
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
