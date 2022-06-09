#include "io.h"

struct led_status_base {
	rt_thread_t handle;
	int pwm_channel;
	uint8_t enabled;
};
typedef struct led_status_base *led_status_base_t;

struct led_fading_thread {
	struct led_status_base thread;
	uint8_t current;
	uint8_t direction;
	uint32_t tick;
};
static struct led_fading_thread fading_status[2];
typedef struct led_fading_thread *led_fading_thread_t;

struct led_blinking_thread {
	struct led_status_base thread;
	uint8_t current;
	uint32_t half_cycle;
};
static struct led_blinking_thread blinking_status[2];
typedef struct led_blinking_thread *led_blinking_thread_t;

extern struct rt_device_pwm *pwm_dev;

static void pwm_set(uint32_t ch, uint8_t percent) {
	rt_err_t ret = rt_pwm_set(pwm_dev, ch, 1000000, (1000000 - 100 * percent * percent));
	if (ret != RT_EOK)
		KPRINTF_COLOR(11, "rt_pwm_set(pwm_dev, %d, %d, %d): return %ld", ch, 1000000, (1000000 - 100 * percent * percent), ret);
}

static void enable_thread(led_status_base_t element) {
	if (!element->enabled) {
		rt_thread_resume(element->handle);
		element->enabled = 1;
	}
}

static void disable_thread(led_status_base_t element) {
	if (element->enabled) {
		element->enabled = 0;
	}
}

static inline int get_channel(enum led_id id) {
	assert(id == LED_RED || id == LED_GREEN);
	if (id == LED_RED)
		return PWM_CH_RED;
	else
		return PWM_CH_GREEN;
}

inline static const char *title(enum led_id id) {
	if (id == LED_RED)
		return "<red led>";
	else if (id == LED_GREEN)
		return "<green led>";
	else
		return "<invalid led>";
}

void led_blink(enum led_id id, uint32_t cycle) {
	_DEV_DEBUG("%s blink: %d", title(id), cycle);
	disable_thread(&fading_status[id].thread);
	blinking_status[id].current = 0;
	blinking_status[id].half_cycle = rt_tick_from_millisecond(cycle / 2);
	enable_thread(&blinking_status[id].thread);
}

void led_fade(enum led_id id, uint32_t cycle) {
	_DEV_DEBUG("%s fade: %lu", title(id), cycle);
	disable_thread(&blinking_status[id].thread);
	fading_status[id].current = 0;
	fading_status[id].direction = 0;
	fading_status[id].tick = rt_tick_from_millisecond(cycle / 100);
	enable_thread(&fading_status[id].thread);
}

void led_static(enum led_id id, uint8_t light_percent) {
	_DEV_DEBUG("%s static level: %d", title(id), light_percent);
	disable_thread(&fading_status[id].thread);
	disable_thread(&blinking_status[id].thread);
	pwm_set(get_channel(id), light_percent);
}

void led_off(enum led_id id) {
	led_static(id, 0);
}

void led_on(enum led_id id) {
	led_static(id, 100);
}

#define CHECK_ENABLE()                           \
	while (!state->thread.enabled) {             \
		rt_thread_suspend(state->thread.handle); \
		rt_schedule();                           \
	}

static void led_blink_thread_main(void *arg) {
	led_blinking_thread_t state = (led_blinking_thread_t)arg;
	int ch = state->thread.pwm_channel;
	while (1) {
		state->current = !state->current;
		pwm_set(ch, state->current ? 100 : 0);
		rt_thread_delay(state->half_cycle);
		CHECK_ENABLE();
	}
}
static void led_fade_thread_main(void *arg) {
	led_fading_thread_t state = (led_fading_thread_t)arg;
	int ch = state->thread.pwm_channel;
	while (1) {
		if (state->current == 0 || state->current == 100)
			state->direction = !state->direction;
		state->current += state->direction ? 1 : -1;
		pwm_set(ch, state->current);
		rt_thread_delay(state->tick);
		CHECK_ENABLE();
	}
}

static void init_base(led_status_base_t thread, enum led_id id, const char *name, void (*entry)(void *parameter), void *parameter, size_t add_stack) {
	thread->enabled = 0;
	thread->pwm_channel = get_channel(id);
	thread->handle = rt_thread_create(name, entry, parameter, add_stack + 512, RT_THREAD_PRIORITY_MAX - 3, 2);
	assert(thread->handle != NULL);
	rt_thread_startup(thread->handle);
	rt_thread_suspend(thread->handle);
}

void led_thread_init() {
	assert0(rt_pwm_enable(pwm_dev, PWM_CH_RED));
	assert0(rt_pwm_enable(pwm_dev, PWM_CH_GREEN));

	init_base(&blinking_status[LED_RED].thread, LED_RED, "red_led_blink", led_blink_thread_main, &blinking_status[LED_RED], 0);
	init_base(&blinking_status[LED_GREEN].thread, LED_GREEN, "green_led_blink", led_blink_thread_main, &blinking_status[LED_GREEN], 0);

	init_base(&fading_status[LED_RED].thread, LED_RED, "red_led_fade", led_fade_thread_main, &fading_status[LED_RED], 128);
	init_base(&fading_status[LED_GREEN].thread, LED_GREEN, "green_led_fade", led_fade_thread_main, &fading_status[LED_GREEN], 128);

	led_static(LED_RED, 0);
	led_static(LED_GREEN, 0);
}
