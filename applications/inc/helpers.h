#pragma once

#include <string.h>

#define ALWAYS_INLINE __attribute__((always_inline)) inline static

ALWAYS_INLINE int str_prefix(const char *pre, size_t n, const char *str)
{
	return strncmp(pre, str, n) == 0;
}

ALWAYS_INLINE int str_eq(const char *str, const char *cmp)
{
	return strcmp(str, cmp) == 0;
}

#define assert0(V) RT_ASSERT(V == 0)

// 输出错误，自动重启
#define FATAL_ERROR(...) __FATAL_ERROR(__FUNCTION__, __LINE__, __VA_ARGS__)
__attribute__((noreturn)) void __FATAL_ERROR(const char *const MSG, ...);

// 不要自动重启
#define PHYSICAL_ERROR(...) __PHYSICAL_ERROR(__FUNCTION__, __LINE__, __VA_ARGS__)
__attribute__((noreturn)) void __PHYSICAL_ERROR(const char *const MSG, ...);
