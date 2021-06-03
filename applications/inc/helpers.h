#pragma once

#include <string.h>
#include <rtconfig.h>

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
#define FATAL_ERROR(...) __FATAL_ERROR(__FILE__, __FUNCTION__, __LINE__, __VA_ARGS__)
__attribute__((noreturn)) void __FATAL_ERROR(const char *file, const char *fn, int lineno, const char *const MSG, ...);

// 不要自动重启
#define PHYSICAL_ERROR(...) __PHYSICAL_ERROR(__FILE__, __FUNCTION__, __LINE__, __VA_ARGS__)
__attribute__((noreturn)) void __PHYSICAL_ERROR(const char *file, const char *fn, int lineno, const char *const MSG, ...);

void outputf(const char *msg, ...);
void outputs(const char *msg);

#ifdef RT_USING_CONSOLE
#define outputf rt_kprintf
#define outputs rt_kputs
#endif

extern char shared_log_buffer[RT_CONSOLEBUF_SIZE];

#if defined(RT_DEBUG_COLOR) || defined(MY_DEBUG_COLOR)
#define KPRINTF_COLOR(COLOR, MSG, ...) outputf("\r\033[38;5;" #COLOR "m" MSG "\033[0m\n", ##__VA_ARGS__)
#define KPRINTF_LIGHT(MSG, ...) (outputf("\r\033[1m" MSG "\033[0m\n", ##__VA_ARGS__))
#define KPRINTF_DIM(MSG, ...) (outputf("\r\033[2m" MSG "\033[0m\n", ##__VA_ARGS__))
#define SET_COLOR_RAW(RAW) (outputs("\033[" RAW "m"))
#else
#define KPRINTF_COLOR(COLOR, MSG, ...) outputf(MSG, ##__VA_ARGS__)
#define KPRINTF_LIGHT(COLOR) outputf(MSG, ##__VA_ARGS__)
#define KPRINTF_DIM(COLOR) outputf(MSG, ##__VA_ARGS__)
#define SET_COLOR_RAW(RAW)
#endif

#define RESET_COLOR() SET_COLOR_RAW("0")
#define SET_COLOR(COLOR) SET_COLOR_RAW("38;5;" #COLOR)
#define SET_BG_COLOR(COLOR) SET_COLOR_RAW("48;5;" #COLOR)
#define SET_COLOR_DIM() SETCOLOR_RAW("2")

__attribute__((noreturn)) void thread_suspend();

#define RT_TICK_FROM_MILLISECOND(ms) (RT_TICK_PER_SECOND * (ms / 1000) + (RT_TICK_PER_SECOND * (ms % 1000) + 999) / 1000)
