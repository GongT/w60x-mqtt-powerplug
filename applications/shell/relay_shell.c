#define DBG_SECTION_NAME "sh:relay"
#include "shell.h"

static long set_relay_msh(int argc, char **argv)
{
	if (str_eq(argv[1], "on"))
		relay_set(1);
	else if (str_eq(argv[1], "off"))
		relay_set(0);
	else
		RETURN_LOG_E("relay on/off");
	return 0;
}
MSH_CMD_EXPORT_ALIAS(set_relay_msh, relay, set relay on / off);
