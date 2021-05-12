#define DBG_SECTION_NAME "gpio.beep"

#include "io.h"

#define BEEP_QUEUE 32

struct beep_state
{
	uint16_t toneHz;
	uint8_t volumePercent;
	uint32_t timeMs;
};

static rt_thread_t thread = NULL;
static rt_mutex_t queue_lock = NULL;
static struct beep_state queue[BEEP_QUEUE];
static struct beep_state current;
static uint8_t start = 0, end = 0;

inline static void _beep_queue(uint16_t toneHz, uint8_t volumePercent, uint32_t timeMs)
{
	if (start == end)
	{
		LOG_E("Beep queue is full");
		return;
	}

	LOG_D("Beep queue start=%d, end=%d", start, end);

	queue[end].toneHz = toneHz;
	queue[end].volumePercent = volumePercent;
	queue[end].timeMs = timeMs;
	end++;
	if (end == BEEP_QUEUE)
		end = 0;
}

void buzzer_beep(uint16_t toneHz, uint8_t volumePercent, uint32_t timeMs)
{
	rt_mutex_take(queue_lock, RT_WAITING_FOREVER);
	_beep_queue(toneHz, volumePercent, timeMs);
	rt_mutex_release(queue_lock);
}

inline static rt_bool_t _pop_queue()
{
	if (end == start) // empty queue
		return RT_FALSE;
	current = queue[start];
	start++;
	if (start == BEEP_QUEUE)
		start = 0;
	return RT_TRUE;
}
inline static rt_bool_t pop_queue()
{
	rt_mutex_take(queue_lock, RT_WAITING_FOREVER);
	rt_bool_t r = _pop_queue();
	rt_mutex_release(queue_lock);
	return r;
}

static void buzzer_play(uint16_t toneHz, uint8_t volumePercent)
{
	if (volumePercent == 0 || toneHz == 0)
	{
		assert0(rt_pwm_set(pwm_dev, PWM_CH_BEEP, 0, 0));
		return;
	}
	LOG_I("beep: %dHz, %d%%", toneHz, volumePercent);
	int period = (1000 * 1000 * 1000 / toneHz);
	int pulse = (((period * 70) / 100) * volumePercent) / 100;
#if BEEP_INVERT
	pulse = period - pulse;
#endif
	assert0(rt_pwm_set(pwm_dev, PWM_CH_BEEP, period, pulse));
}

static void buzzer_play_delay(uint16_t toneHz, uint8_t volumePercent, uint32_t timeMs)
{
	buzzer_play(toneHz, volumePercent);
	rt_thread_mdelay(timeMs);
	buzzer_play(0, 0);
}

// void config_mode_status_hook(enum CONFIG_STATUS why)
// {
// 	LOG_I("config mode status: %d", why);
// }

static void beep_thread_main(void *args)
{
	LOG_I("beep thread started.");
	while (1)
	{
		if (pop_queue())
			buzzer_play_delay(current.toneHz, current.volumePercent, current.timeMs);
		else
			rt_thread_yield();
	}
}

void beep_thread_init()
{
	assert0(rt_pwm_enable(pwm_dev, PWM_CH_BEEP));

	queue_lock = rt_mutex_create("beep_queue", RT_IPC_FLAG_FIFO);
	assert(queue_lock != NULL);

	thread = rt_thread_create("beep", beep_thread_main, NULL, 256, 1, 10);
	assert(thread != NULL);
	rt_thread_startup(thread);
}

void beep_error()
{
	rt_mutex_take(queue_lock, RT_WAITING_FOREVER);
	buzzer_play(200, 10);
}
