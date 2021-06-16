#pragma once

#include <string.h>
#include <rtconfig.h>
#include <rt-thread-w60x/helper.h>

// 输出错误，自动重启
#define FATAL_ERROR(...) __FATAL_ERROR(__FILE__, __FUNCTION__, __LINE__, __VA_ARGS__)
__attribute__((noreturn)) void __FATAL_ERROR(const char *file, const char *fn, int lineno, const char *const MSG, ...);

// 不要自动重启
#define PHYSICAL_ERROR(...) __PHYSICAL_ERROR(__FILE__, __FUNCTION__, __LINE__, __VA_ARGS__)
__attribute__((noreturn)) void __PHYSICAL_ERROR(const char *file, const char *fn, int lineno, const char *const MSG, ...);

extern char shared_log_buffer[RT_CONSOLEBUF_SIZE];
