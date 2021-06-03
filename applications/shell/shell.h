#pragma once

#include "app.h"
#ifndef RT_USING_FINSH
#error "RT_USING_FINSH is not set"
#endif

#include <finsh.h>

typedef long (*shell_handler)(int argc, char **argv);

#define RETURN_WITH_ERR(...)             \
	{                                    \
		KPRINTF_COLOR(9, ##__VA_ARGS__); \
		return 1;                        \
	}

#define FORCE_CAST_SYSCALL(FN) (*((syscall_func)(void *)&FN))
#define DEFINE_CMD(FN, CMD, DESC) MSH_CMD_EXPORT_ALIAS(FORCE_CAST_SYSCALL(FN), CMD, DESC);
