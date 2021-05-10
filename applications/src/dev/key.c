#define DBG_SECTION_NAME "gpio.key"

#include "io.h"

#if RT_TICK_PER_SECOND < 10
#error "clock is too slow"
#endif

#define PRESS_MAX_DELAY_TICK (PRESS_MAX_DELAY_MS / (1000 / RT_TICK_PER_SECOND))
#define DBLCLICK_MAX_DELAY_TICK (DBLCLICK_MAX_DELAY_MS / (1000 / RT_TICK_PER_SECOND))

rt_bool_t key_press_status = RT_FALSE;
uint32_t last_down = 0;
uint32_t last_up = 0;
uint32_t this_up = 0;

rt_bool_t key_is_pressed()
{
	return key_press_status;
}

static void check(void *args)
{
	key_press_status = rt_pin_read(PIN_KEY) == 0;
	if (key_press_status)
	{
	}
	else
	{
	}
}

void key_press_init()
{
	rt_pin_mode(PIN_KEY, PIN_MODE_INPUT_PULLUP);

	key_press_status = rt_pin_read(PIN_KEY) == 0;
	if (key_press_status)
		LOG_W("key init is pressed");
	else
		LOG_W("key init is not pressed");

	rt_pin_attach_irq(PIN_KEY, PIN_IRQ_MODE_RISING_FALLING, check, NULL);
}
