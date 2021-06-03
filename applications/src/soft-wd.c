#include "app.h"
#include <stdlib.h>

static rt_timer_t tmr = NULL;
static rt_thread_t thread = NULL;
static rt_tick_t to;
static rt_tick_t to3;
#define TIMEOUT_MS 5000

extern void list_thread();
extern void list_device();
extern void list_mutex();
extern void list_timer();
extern void list_mempool();
extern void list_msgqueue();
extern void list_mailbox();
extern void list_event();
extern void list_sem();

static void handle_task_kill(void *args)
{
	KPRINTF_COLOR(9, "some thread delay");

#ifdef RT_USING_FINSH
	outputs("=================================\n");
	list_thread();
	list_device();
	list_mutex();
	list_timer();
	list_mempool();
	list_msgqueue();
	list_mailbox();
	list_event();
	list_sem();
	outputs("=================================\n");
#endif
}

static void schedule_record(void *args)
{
	while (1)
	{
		// KPRINTF_DIM("-- schedule_record");
		if (rt_timer_stop(tmr) != RT_EOK)
		{
			KPRINTF_COLOR(9, "* timer stop fail");
		}
		if (rt_timer_start(tmr) != RT_EOK)
		{
			KPRINTF_COLOR(9, "* timer start fail");
		}
		rt_thread_delay(to);
	}
}
static int task_killer_init()
{
	to = rt_tick_from_millisecond(TIMEOUT_MS);
	to3 = to * 3;

	tmr = rt_timer_create("th_timeout", handle_task_kill, NULL, to3, RT_TIMER_FLAG_HARD_TIMER + RT_TIMER_FLAG_ONE_SHOT);
	assert(tmr != NULL);
	assert(rt_timer_start(tmr) == RT_EOK);

	thread = rt_thread_create("th_active", schedule_record, NULL, 256 + 128, 5, 50);
	assert(thread != NULL);
	assert(rt_thread_startup(thread) == RT_EOK);

	return 0;
}
INIT_APP_EXPORT(task_killer_init);
