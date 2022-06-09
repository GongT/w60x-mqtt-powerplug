#include "shell.h"
#include <stdlib.h>
#include <drv_flash.h>
#include <wm_flash_map.h>
#include <wm_internal_flash.h>

#define USAGE_RETURN(MSG, ...)                                             \
	do {                                                                   \
		if (strlen(MSG))                                                   \
			rt_kprintf(MSG "\n", ##__VA_ARGS__);                           \
		rt_kputs("flash read <from address> [size]\n");                    \
		rt_kputs("flash erase <address> [chunk count]\n");                 \
		rt_kputs("flash continue/retry\n");                                \
		rt_kputs("flash write <to address> <hex string without space>\n"); \
		return 1;                                                          \
	} while (0)

static long continue_from = 0;
static size_t continue_size = 0;
static int flash_read(unsigned long from, size_t size) {
	if (from < FLASH_BASE_ADDR) {
		from += FLASH_BASE_ADDR;
	}
	KPRINTF_COLOR(2, "read from %lX size %d (end: %lX) ...\n", from, size, from + size);
	if (size == 0 || size > 1024) {
		KPRINTF_COLOR(9, "can not read %d bytes, allow 0~1024 bytes\n", size);
		return RT_EINVAL;
	}

	continue_from = from;
	continue_size = size;

	uint8_t buff[size + 1];
	memset(buff, 0, sizeof(buff));
	int ret = tls_fls_read(from, buff, size);

	rt_kputs("\n");
	uint i;
	for (i = 0; i < size; i++) {
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
static int flash_erase(unsigned long from, size_t size) {
	if (from % INSIDE_FLS_SECTOR_SIZE != 0) {
		KPRINTF_COLOR(9, "can not erase at 0x%lX, not align %lu\n", from, INSIDE_FLS_SECTOR_SIZE);
		return RT_EINVAL;
	}
	unsigned int sec = from / INSIDE_FLS_SECTOR_SIZE;
	while (size-- > 0) {
		KPRINTF_COLOR(2, "erase sector %d\n", sec);
		int ret = tls_fls_erase(sec);
		if (ret != TLS_FLS_STATUS_OK)
			KPRINTF_COLOR(9, " * failed erase this sector\n");
		sec++;
	}
	return 0;
}

static long flash_rw_msh(int argc, char **argv) {
	if (argc < 2)
		USAGE_RETURN("");

	int ret;
	if (str_prefix("read", argv[1])) {
		if (argc == 3)
			ret = flash_read(strtoul(argv[2], NULL, 0), 16);
		else if (argc == 4)
			ret = flash_read(strtoul(argv[2], NULL, 0), (size_t)strtoul(argv[3], NULL, 10));
		else
			USAGE_RETURN("address is required!");
	} else if (str_prefix("erase", argv[1])) {
		if (argc == 3)
			ret = flash_erase(strtoul(argv[2], NULL, 0), 16);
		else if (argc == 4)
			ret = flash_erase(strtoul(argv[2], NULL, 0), (size_t)strtoul(argv[3], NULL, 10));
		else
			USAGE_RETURN("address is required!");
	} else {
		USAGE_RETURN("invalid command: %s", argv[1]);
	}

	if (ret != 0) {
		KPRINTF_COLOR(11, "function return: %d\n", ret);
	}

	return ret;
}
DEFINE_CMD(flash_rw_msh, flash, get_key_status);
