#define DBG_SECTION_NAME "gpio.key"
#define DBG_LVL DBG_ERROR

#include "io.h"

#if RT_TICK_PER_SECOND < 10
#error "clock is too slow"
#endif

#define PRESS_MAX_DELAY_TICK (PRESS_MAX_DELAY_MS / (1000 / RT_TICK_PER_SECOND))
#define DBLCLICK_MAX_DELAY_TICK (DBLCLICK_MAX_DELAY_MS / (1000 / RT_TICK_PER_SECOND))

static rt_bool_t key_press_status = RT_FALSE;
static rt_tick_t last_event_time = 0;
static rt_mailbox_t event_msg_queue;
static rt_thread_t reduce_thread;

rt_bool_t key_is_pressed()
{
	return key_press_status;
}

ALWAYS_INLINE rt_bool_t check()
{
	return rt_pin_read(PIN_KEY) == KEY_DOWN_IS;
}

static void key_handler(void *args)
{
	rt_bool_t new_status = check();
	if (key_press_status == new_status)
		return;
	key_press_status = new_status;

	rt_tick_t this_time = rt_tick_get_millisecond();
	rt_tick_t delta = this_time - last_event_time;
	last_event_time = this_time;

	if (delta == 0 || delta == 1)
		return;

	uint32_t msg = (delta & 0x7FFFFFFF) + (key_press_status ? 0x80000000 : 0);
	LOG_I("key raw event: %d", key_press_status);
	rt_mb_send(event_msg_queue, msg);
}

__attribute__((noreturn)) static void key_event_reduce_main(void *arg)
{
	rt_int32_t timeout = RT_WAITING_FOREVER;
	uint32_t store;
	rt_bool_t wait_double_click = RT_FALSE, wait_long_press = RT_FALSE;

	while (1)
	{
		rt_bool_t is_timeout = rt_mb_recv(event_msg_queue, &store, timeout) != RT_EOK;
		if (is_timeout)
		{
			LOG_I("timeout: wait_double_click=%d, wait_long_press=%d", wait_double_click, wait_long_press);
			timeout = RT_WAITING_FOREVER;
			if (wait_double_click)
			{
				wait_double_click = RT_FALSE;
				LOG_E("todo: single_click");
			}
			else if (wait_long_press)
			{
				wait_long_press = RT_FALSE;
				LOG_E("todo: long_press");
			}
			continue;
		}

		// rt_tick_t delta_ms = store & 0x7FFFFFFF;
		rt_bool_t is_key_down = (store & 0x80000000) != 0;

		if (is_key_down)
		{
			LOG_I("down   : wait_double_click=%d, wait_long_press=%d", wait_double_click, wait_long_press);
			if (wait_double_click)
			{
				wait_double_click = RT_FALSE;
				timeout = RT_WAITING_FOREVER;
				LOG_E("todo: double_click");
			}
			else
			{
				LOG_I("+wait_long_press");
				wait_long_press = RT_TRUE;
				timeout = RT_TICK_FROM_MILLISECOND(LONG_PRESS_HOLD_MS);
			}
		}
		else
		{
			LOG_I("up     : wait_double_click=%d, wait_long_press=%d", wait_double_click, wait_long_press);
			if (wait_long_press)
			{
				LOG_I("+wait_double_click   -wait_long_press");
				wait_long_press = RT_FALSE;
				wait_double_click = RT_TRUE;
				timeout = RT_TICK_FROM_MILLISECOND(DBLCLICK_MAX_DELAY_MS);
			}
		}
	}
}

void key_press_init()
{
	rt_pin_mode(PIN_KEY, PIN_MODE_INPUT_PULLUP);

	key_press_status = check();
	if (key_press_status)
	{
		LOG_W("key0 pin (%d) init state is pressed", PIN_KEY);
	}
	else
	{
		LOG_W("key0 pin (%d) init state is not pressed", PIN_KEY);
	}

	event_msg_queue = rt_mb_create("key_event", 64, RT_IPC_FLAG_FIFO);
	assert(event_msg_queue != NULL);

	reduce_thread = rt_thread_create("key_reducer", key_event_reduce_main, NULL, 512, 5, 2);
	assert(reduce_thread != NULL);

	assert0(rt_thread_startup(reduce_thread));

	assert0(rt_pin_attach_irq(PIN_KEY, PIN_IRQ_MODE_RISING_FALLING, key_handler, NULL));
	assert0(rt_pin_irq_enable(PIN_KEY, PIN_IRQ_ENABLE));
}
