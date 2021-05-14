#define DBG_SECTION_NAME "sh:key"
#include "shell.h"

static long key_press_status_msh(int argc, char **argv)
{
	int v = rt_pin_read(PIN_KEY);
	if (key_is_pressed())
		LOG_I("key is pressed. (raw value is %d)", v);
	else
		LOG_I("key is not pressed. (raw value is %d)", v);
	return 0;
}
MSH_CMD_EXPORT_ALIAS(key_press_status_msh, get_key, get_key_status);
