#include "app.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#undef rt_kprintf
#undef rt_kputs

#define MAX_BOOT_LOG_MEM 10240

static rt_device_t uart;
static char *log_buff = NULL;

static int find_device() {
	uart = rt_device_find("uart0");

	assert(rt_device_open(uart, RT_DEVICE_OFLAG_RDWR | RT_DEVICE_FLAG_STREAM) == RT_EOK);

	assert(uart != NULL);

	if (log_buff != NULL) {
		rt_device_write(uart, 0, log_buff, strlen(log_buff));
		free(log_buff);
		log_buff = NULL;
	}

	return 0;
}
INIT_BOARD_EXPORT(find_device);

void rt_hw_console_output(const char *msg) {
	if (uart == NULL) {
		size_t current = strlen(log_buff);
		if (log_buff == NULL)
			log_buff = malloc(MAX_BOOT_LOG_MEM);
		strncpy(log_buff + current, msg, MAX_BOOT_LOG_MEM - 1 - current);
	} else {
		rt_device_write(uart, 0, msg, rt_strlen(msg));
	}
}
