#include "shell.h"

static long key_press_status_msh(int argc, char **argv)
{
	int v = rt_pin_read(PIN_KEY);
	if (key_is_pressed())
		KPRINTF_DIM("key is pressed. (raw value is %d)", v);
	else
		KPRINTF_DIM("key is not pressed. (raw value is %d)", v);
	return 0;
}
DEFINE_CMD(key_press_status_msh, get_key, get key0 pressed or not );
