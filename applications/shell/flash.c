#define DBG_SECTION_NAME "sh:flash"

#include "shell.h"
#include <stdlib.h>
#include <drv_flash.h>
#include <wm_flash_map.h>
#include <wm_internal_flash.h>

#define USAGE_RETURN(MSG, ...)                                           \
	do                                                                   \
	{                                                                    \
		if (strlen(MSG))                                                 \
			rt_kprintf(MSG "\n", ##__VA_ARGS__);                         \
		rt_kprintf("flash read <from address> [size]\n");                \
		rt_kprintf("flash continue/retry\n");                            \
		rt_kputs("led write <to address> <hex string without space>\n"); \
		return 1;                                                        \
	} while (0)

static long continue_from = 0;
static size_t continue_size = 0;
static int flash_read(unsigned long from, size_t size)
{
	if (from < FLASH_BASE_ADDR)
	{
		from += FLASH_BASE_ADDR;
	}
	KPINTF_COLOR(2, "read from %X size %d (end: $X) ...\n", from, size, from + size);
	if (size == 0 || size > 1024)
	{
		KPINTF_COLOR(9, "can not read %d bytes, allow 0~1024 bytes\n", size);
		return RT_EINVAL;
	}

	continue_from = from;
	continue_size = size;

	uint8_t buff[size + 1];
	memset(buff, 0, sizeof(buff));
	int ret = tls_fls_read(from, buff, size);

	rt_kputs("\n");
	uint i;
	for (i = 0; i < size; i++)
	{
		rt_kprintf("%02X ", buff[i] & 0xff);
		if ((i + 1) % 16 == 0)
			rt_kputs("\n");
		else if ((i + 1) % 8 == 0)
			rt_kputs("  ");
	}
	if ((i + 1) % 16 != 0)
		rt_kputs("\n");

	return ret;
}

static long flash_rw_msh(int argc, char **argv)
{
	if (argc < 2)
		USAGE_RETURN("");

	int ret;
	if (str_prefix("read", argv[1]))
	{
		if (argc == 3)
			ret = flash_read(strtoul(argv[2], NULL, 0), 16);
		else if (argc == 4)
			ret = flash_read(strtoul(argv[2], NULL, 0), (size_t)strtoul(argv[3], NULL, 10));
		else
			USAGE_RETURN("size is required!");
	}
	else
	{
		USAGE_RETURN("invalid command: %s", argv[1]);
	}

	if (ret != 0)
	{
		KPINTF_COLOR(11, "function return: %d\n", ret);
	}

	return ret;
}
MSH_CMD_EXPORT_ALIAS(flash_rw_msh, flash, get_key_status);
