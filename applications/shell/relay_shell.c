#include "shell.h"

static long set_relay_msh(int argc, char **argv)
{
	if (str_eq(argv[1], "on"))
		relay_set(RELAY_ON);
	else if (str_eq(argv[1], "off"))
		relay_set(RELAY_OFF);
	else
		RETURN_WITH_ERR("relay on/off");
	return 0;
}
DEFINE_CMD(set_relay_msh, relay, set relay on / off);
