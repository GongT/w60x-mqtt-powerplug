#include <stdarg.h>
#include <string.h>
#include <stdio.h>

#include "helpers.h"
#include "app.h"
#include <rthw.h>
#include <ulog.h>

char shared_log_buffer[RT_CONSOLEBUF_SIZE];

static void shutdown()
{
	KPRINTF_COLOR(9, "busy loop soft stop.\n");
	while (1)
		;
}

__attribute__((noreturn)) void thread_suspend()
{
	rt_thread_t self = rt_thread_self();
	if (rt_object_get_type((rt_object_t)self) != RT_Object_Class_Thread)
	{
		KPRINTF_COLOR(9, "thread object memory area has been destroyed, halt cpu now.\n");
		shutdown();
		while (1)
			;
	}

	register rt_base_t temp = rt_hw_interrupt_disable();

	sprintf(shared_log_buffer, "%s(halt)", self->name);
	strncpy(self->name, shared_log_buffer, RT_NAME_MAX);

	rt_hw_interrupt_enable(temp);

	// #ifdef RT_USING_FINSH
	// 	rt_thread_mdelay(1000);
	// 	list_thread();
	// 	puts("\n");
	// #endif

	rt_thread_suspend(self);
	rt_schedule();
	KPRINTF_COLOR(9, "who wake me?!");

	while (1)
		rt_thread_mdelay(10000);
}

__attribute__((noreturn)) static void suspend()
{
	rt_thread_t self = rt_thread_self();
	if (self == RT_NULL)
	{
		KPRINTF_COLOR(9, "suspend in ISR, halt cpu now.\n");
		shutdown();
		while (1)
			;
	}
	else
		thread_suspend();
}

__attribute__((noreturn)) static void reboot()
{
#if DISABLE_REBOOT
	KPRINTF_COLOR(14, "reboot disabled, suspend instead.\n");
	suspend();
#else
	rt_thread_mdelay(5000);
	rt_hw_cpu_reset();
#endif
}

__attribute__((noreturn)) void __FATAL_ERROR(const char *file, const char *fn, int lineno, const char *const MSG, ...)
{
	va_list argptr;
	va_start(argptr, MSG);
	rt_vsprintf(shared_log_buffer, MSG, argptr);
	va_end(argptr);

	SET_COLOR(9);
	printf("\r[FATAL_ERROR] @ %s + ", fn);
	printf(file);
	printf(":%d\n", lineno);
	puts(shared_log_buffer);
	RESET_COLOR();

	show_context();

	beep_error();
	led_off(LED_GREEN);
	led_blink(LED_RED, 300);

	reboot();
}

__attribute__((noreturn)) void __PHYSICAL_ERROR(const char *file, const char *fn, int lineno, const char *const MSG, ...)
{
	va_list argptr;
	va_start(argptr, MSG);
	rt_vsprintf(shared_log_buffer, MSG, argptr);
	va_end(argptr);

	SET_COLOR(9);
	printf("\r[PHYSICAL_ERROR] @ %s + ", fn);
	printf(file);
	printf(":%d\n", lineno);
	puts(shared_log_buffer);

	show_context();

	beep_error();
	led_on(LED_GREEN);
	led_on(LED_RED);

	suspend();
}

static void rtt_user_assert_hook(const char *ex, const char *func, rt_size_t line)
{
	KPRINTF_COLOR(9, "\r[ASSERT FAILED] @ %s:%ld.", func, line, ex);
	KPRINTF_COLOR(9, " * assert condition: %s\n", ex);

	beep_error();

	show_context();

	reboot();
}

static int set_assert()
{
	rt_assert_set_hook(rtt_user_assert_hook);
	return 0;
}
INIT_BOARD_EXPORT(set_assert);
