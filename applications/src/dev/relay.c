#include "io.h"

static void queue_publish(const char *msg)
{
	static main_event body = {REPORT_RELAY, 0, NULL};
	body.payload = msg;
	rt_err_t r = rt_mq_send(main_events, (void *)&body, sizeof(main_event));
	if (r != RT_EOK)
		KPRINTF_COLOR(11, "relay: rt_mq_send: return %d", r);
}

void relay_set(enum relay_state_t on)
{
	KPRINTF_DIM("relay: %d", on);
	rt_pin_write(PIN_RELAY, on);
	queue_publish(on ? "1" : "0");
}

void relay_init()
{
	rt_pin_mode(PIN_RELAY, PIN_MODE_OUTPUT);
	relay_set(RELAY_OFF);
}
