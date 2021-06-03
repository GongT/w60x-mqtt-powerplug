#include "io.h"

void relay_set(enum relay_state_t on)
{
	KPRINTF_DIM("relay: %d", on);
	rt_pin_write(PIN_RELAY, on);
}

void relay_init()
{
	rt_pin_mode(PIN_RELAY, PIN_MODE_OUTPUT);
	relay_set(RELAY_OFF);
}
