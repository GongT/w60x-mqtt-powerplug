#pragma once

#if !defined(DBG_TAG) && !defined(DBG_SECTION_NAME)
#warning "此文件没有定义C语言常量DBG_TAG或DBG_SECTION_NAME，默认为unknown"
#define DBG_TAG "main"
#endif
#ifndef DBG_LVL
#define DBG_LVL DBG_LOG
#endif
#include <rtdbg.h>

#include <string.h>

#define ALWAYS_INLINE __attribute__((always_inline)) inline static

#define str_prefix(string, find) str_nprefix(string, strlen(find), find)
ALWAYS_INLINE int str_nprefix(const char *string, size_t max_char, const char *find)
{
	return strncmp(string, find, max_char) == 0;
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

#ifdef RT_DEBUG_COLOR
#define KPINTF_COLOR(COLOR, MSG, ...) rt_kprintf("\033[38;5;" #COLOR "m" MSG "\033[0m", ##__VA_ARGS__)
#else
#define KPINTF_COLOR(COLOR, MSG, ...) rt_kprintf(MSG, ##__VA_ARGS__)
#endif

#ifdef RT_DEBUG_COLOR
#define SET_COLOR(COLOR) rt_kputs("\033[38;5;" #COLOR "m")
#else
#define SET_COLOR(COLOR)
#endif

#define RESET_COLOR(COLOR) SET_COLOR(0)

__attribute__((noreturn)) void thread_suspend();

#define RT_TICK_FROM_MILLISECOND(ms) (RT_TICK_PER_SECOND * (ms / 1000) + (RT_TICK_PER_SECOND * (ms % 1000) + 999) / 1000)
