#include "app.h"

#undef outputf
#undef outputs
#undef rt_kprintf
#undef rt_kputs

static rt_device_t uart;

static int find_device()
{
	uart = rt_device_find("uart0");
	assert(uart != NULL);

#ifndef RT_USING_CONSOLE
	assert(rt_device_open(uart, RT_DEVICE_OFLAG_RDWR | RT_DEVICE_FLAG_STREAM) == RT_EOK);
#endif

	return 0;
}
INIT_BOARD_EXPORT(find_device);

void rt_hw_console_output(const char *msg)
{
	rt_device_write(uart, 0, msg, rt_strlen(msg));
}

void outputs(const char *msg)
{
	rt_hw_console_output(msg);
}

void outputf(const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	rt_int32_t length = rt_vsnprintf(shared_log_buffer, RT_CONSOLEBUF_SIZE, fmt, args);
	va_end(args);

	rt_device_write(uart, 0, shared_log_buffer, length);
}
