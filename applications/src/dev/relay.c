#include "io.h"

void relay_set(enum relay_state_t on)
{
	_DEV_DEBUG("relay: %d", on);
	rt_pin_write(PIN_RELAY, on);
	main_event_queue(REPORT_RELAY, on ? "ON" : "OFF", RT_FALSE);
}

void relay_init()
{
	rt_pin_mode(PIN_RELAY, PIN_MODE_OUTPUT);
	relay_set(RELAY_OFF);
}
