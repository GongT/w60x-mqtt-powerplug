#define DBG_SECTION_NAME "helper"

#include <stdarg.h>
#include <stdio.h>

#include "helpers.h"
#include "app.h"
#include <rthw.h>
#include <ulog.h>

__attribute__((noreturn)) void __FATAL_ERROR(const char *const MSG, ...)
{
	rt_enter_critical();

	printf("\n[FATAL_ERROR] ");

	va_list argptr;
	va_start(argptr, MSG);
	vprintf(MSG, argptr);
	va_end(argptr);

	printf("\n");
	ulog_flush();

	rt_thread_mdelay(5000);
	if (!DISABLE_REBOOT)
		rt_hw_cpu_reset();
	while (1)
		;
}

__attribute__((noreturn)) void __PHYSICAL_ERROR(const char *const MSG, ...)
{
	beep_error();

	rt_enter_critical();

	va_list argptr;
	va_start(argptr, MSG);
	while (1)
	{
		printf("\n[PHYSICAL_ERROR] ");
		vprintf(MSG, argptr);
		printf("\n");
		ulog_flush();
		rt_thread_mdelay(5000);
	}
	va_end(argptr);
}

static void rtt_user_assert_hook(const char *ex, const char *func, rt_size_t line)
{
	beep_error();

	rt_enter_critical();

	ulog_output(LOG_LVL_ASSERT, LOG_TAG, RT_TRUE, "(%s) has assert failed at %s:%ld.", ex, func, line);
	/* flush all log */
	ulog_flush();
	while (1)
		;
}

static int set_assert()
{
	rt_assert_set_hook(rtt_user_assert_hook);
	return 0;
}

INIT_PREV_EXPORT(set_assert);
