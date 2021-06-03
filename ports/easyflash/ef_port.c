#include <easyflash.h>
#include <stdlib.h>
#include <stdarg.h>
#include <rtthread.h>
#include "ef_cfg.h"
#include "helpers.h"

static const ef_env default_env_set[] = {
	{"boot_times", "0"},
};
extern char *global_buffer;
static struct rt_semaphore env_cache_lock;

rt_bool_t easy_flash_extra_init();

EfErrCode ef_port_init(ef_env const **default_env, size_t *default_env_size)
{
	*default_env = default_env_set;
	*default_env_size = sizeof(default_env_set) / sizeof(default_env_set[0]);

	if (rt_sem_init(&env_cache_lock, "env lock", 1, RT_IPC_FLAG_PRIO) != RT_EOK)
		return EF_ENV_INIT_FAILED;

	if (!easy_flash_extra_init())
		return EF_ENV_INIT_FAILED;

	return EF_NO_ERR;
}

void ef_port_env_lock(void)
{
	rt_sem_take(&env_cache_lock, RT_WAITING_FOREVER);
}

void ef_port_env_unlock(void)
{
	rt_sem_release(&env_cache_lock);
}

void ef_log_debug(const char *file, const long line, const char *format, ...)
{
	va_list args;

	outputf("[Flash] (%s:%ld) ", file, line);

	va_start(args, format);
	rt_vsnprintf(shared_log_buffer, RT_CONSOLEBUF_SIZE, format, args);
	va_end(args);

	outputs(shared_log_buffer);
}

void ef_log_info(const char *format, ...)
{
	va_list args;

	outputs("[Flash] ");

	va_start(args, format);
	rt_vsnprintf(shared_log_buffer, RT_CONSOLEBUF_SIZE, format, args);
	va_end(args);

	outputs(shared_log_buffer);
}

void ef_print(const char *format, ...)
{
	va_list args;

	va_start(args, format);
	rt_vsnprintf(shared_log_buffer, RT_CONSOLEBUF_SIZE, format, args);
	va_end(args);

	outputs(shared_log_buffer);
}
