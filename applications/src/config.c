#include "app.h"
#include <gongt/config_tool.h>

__attribute__((noreturn)) extern void rt_hw_cpu_reset();

__attribute__((noreturn)) static void reboot()
{
	rt_enter_critical();
	rt_kprintf("\n");
	for (int i = 10; i > 0; i--)
	{
		KPRINTF_COLOR(9, "system will reboot in %ds...", i);
		rt_thread_mdelay(1000);
	}

	KPRINTF_COLOR(9, "reboot...");
	rt_hw_cpu_reset();
	while (1)
		;
}

enum CONFIG_STATUS goto_config_mode_with_alert()
{
	rt_wlan_unregister_event_handler(RT_WLAN_EVT_STA_DISCONNECTED);

	alert_config_start();

	enum CONFIG_STATUS ret = goto_config_mode();
	if (ret == CONFIG_STATUS_SUCCESS)
		alert_config_end();
	else if (ret == CONFIG_STATUS_REBOOT_REQUIRED)
		reboot();
	else
		alert_config_fail();

	return ret;
}

static void goto_config_mode_thread_main()
{
	goto_config_mode_with_alert();
#if DISABLE_REBOOT
	KPRINTF_COLOR(9, "development mode, skip reboot()");
#else
	reboot();
#endif
}

__attribute__((noreturn)) void goto_config_mode_and_quit()
{
	KPRINTF_DIM("going to config mode.");

#if AUTO_GOTO_CONFIG
	rt_thread_t thread = rt_thread_create("config-mode", goto_config_mode_thread_main, NULL, 8192, rt_thread_self()->current_priority, 10);
	rt_thread_startup(thread);
	rt_thread_exit();
#else
	KPRINTF_COLOR(9, "development mode, skip goto_config_mode(), main thread will NOT continue!");
	rt_thread_exit();
#endif
}
