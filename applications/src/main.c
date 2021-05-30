#define DBG_SECTION_NAME "main"

#include "app.h"
#include <easyflash.h>
#include <fal.h>
#include <stdlib.h>
#include "init.h"
#include <gongt/config_tool.h>

static void switch_priority()
{
	uint8_t p = 25;
	rt_thread_control(rt_thread_self(), RT_THREAD_CTRL_CHANGE_PRIORITY, &p);
}

int main(void)
{
	LOG_I("Main function has called...");
	switch_priority();

	if (fal_init() == 0)
		PHYSICAL_ERROR("No partitions found!");

#if !FAL_DEBUG
	fal_show_part_table();
#endif

	LOG_I("easyflash_init()");
	if (easyflash_init() != EF_NO_ERR)
		PHYSICAL_ERROR("Failed to init easyflash!");

	LOG_I("test_env()");
	test_env();

	LOG_I("connect_wifi()");
	if (connect_wifi() != 0)
		goto_config_mode_and_quit();

	LOG_I("start_mqtt()");
	if (start_mqtt() != 0)
		FATAL_ERROR("failed start mqtt");

	thread_suspend();

	return 0;
}
