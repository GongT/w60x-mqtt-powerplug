#pragma once

#include "app.h"
#ifndef RT_USING_FINSH
#error "RT_USING_FINSH is not set"
#endif

#include <finsh.h>

#define RETURN_LOG_E(...)   \
	{                       \
		LOG_E(__VA_ARGS__); \
		return 1;           \
	}
