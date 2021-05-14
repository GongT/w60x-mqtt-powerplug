#define DBG_SECTION_NAME "helper"

#include <stdarg.h>
#include <string.h>
#include <stdio.h>

#include "helpers.h"
#include "app.h"
#include <rthw.h>
#include <ulog.h>

extern void list_thread();

static char log_buf[RT_CONSOLEBUF_SIZE];

__attribute__((noreturn)) void thread_suspend()
{
	rt_thread_t self = rt_thread_self();

	register rt_base_t temp = rt_hw_interrupt_disable();

	sprintf(log_buf, "%s(halt)", self->name);
	strncpy(self->name, log_buf, RT_NAME_MAX);

	rt_hw_interrupt_enable(temp);

#ifdef RT_USING_FINSH
	rt_thread_mdelay(1000);
	list_thread();
	rt_kputs("\n");
#endif

	rt_thread_suspend(self);
	rt_schedule();
	LOG_E("who wake me?!");

	while (1)
		rt_thread_mdelay(10000);
}

__attribute__((noreturn)) static void suspend()
{
	rt_thread_t self = rt_thread_self();
	if (self == RT_NULL)
	{
		rt_hw_cpu_shutdown();
		while (1)
			;
	}
	else
		thread_suspend();
}

__attribute__((noreturn)) static void reboot()
{
#if DISABLE_REBOOT
	KPINTF_COLOR(14, "reboot disabled, suspend instead.\n");
	suspend();
#else
	rt_thread_mdelay(5000);
	rt_hw_cpu_reset();
#endif
}
static void show_context()
{
	SET_COLOR(7);
	rt_thread_t self = rt_thread_self();
	if (self == RT_NULL)
		rt_kputs(" * thread context: no.\n");
	else
		rt_kprintf(" * thread context: [%d] %s.\n", self->stat, self->name);
#ifdef RT_USING_FINSH
	rt_kputs("=================================\n");
	list_thread();
	rt_kputs("=================================\n");
#endif
	RESET_COLOR();
}

__attribute__((noreturn)) void __FATAL_ERROR(const char *const MSG, ...)
{
	va_list argptr;
	va_start(argptr, MSG);
	rt_vsprintf(log_buf, MSG, argptr);
	va_end(argptr);

	SET_COLOR(9);
	rt_kputs("\n[FATAL_ERROR] ");
	rt_kputs(log_buf);
	rt_kputs("\n");
	RESET_COLOR();

	show_context();

	beep_error();

	reboot();
}

__attribute__((noreturn)) void __PHYSICAL_ERROR(const char *const MSG, ...)
{
	va_list argptr;
	va_start(argptr, MSG);
	rt_vsprintf(log_buf, MSG, argptr);
	va_end(argptr);

	SET_COLOR(9);
	rt_kputs("\n[PHYSICAL_ERROR] ");
	rt_kputs(log_buf);
	rt_kputs("\n");

	show_context();

	beep_error();

	suspend();
}

static void rtt_user_assert_hook(const char *ex, const char *func, rt_size_t line)
{
	KPINTF_COLOR(9, "\n\n[ASSERT FAILED] @ %s:%ld.\n", func, line, ex);
	KPINTF_COLOR(9, " * assert condition: %s\n", ex);

	beep_error();

	show_context();

	reboot();
}

static int set_assert()
{
	rt_assert_set_hook(rtt_user_assert_hook);
	return 0;
}

INIT_PREV_EXPORT(set_assert);
