#include "shell.h"
#include <easyflash.h>

static void msh_getenv(uint8_t argc, char **argv)
{
	if (argc < 2)
	{
		rt_kprintf("usage: %s <key name>\n", argv[0]);
		return;
	}

	size_t size = get_storage_size(argv[1]);
	rt_kprintf("%s [length=%d]: ", argv[1], size);

	if (size == 0)
	{
		rt_kputs("*no this value*\n");
		return;
	}

	char buff[size + 1];
	memset(buff, 0, size + 1);
	ef_get_env_blob(argv[1], buff, size + 1, NULL);

	rt_kputs("'");
	rt_kputs(buff);
	rt_kputs("'\n");
}
DEFINE_CMD(getenv, msh_getenv, Get an envrionment variable by name.);
