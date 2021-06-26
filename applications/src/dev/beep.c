#include "io.h"

#define BEEP_QUEUE 10

static rt_thread_t thread = NULL;
static rt_mq_t queue = NULL;

void buzzer_beep(uint16_t toneHz, uint8_t volumePercent, uint32_t timeMs)
{
	static beep_state s;
	s.toneHz = toneHz;
	s.volumePercent = volumePercent;
	s.timeMs = timeMs;

	if (rt_mq_send(queue, &s, sizeof(beep_state)) != RT_EOK)
	{
		rt_kputs("beep queue push failed. maybe full?\n");
	}
}

static void buzzer_play(uint16_t toneHz, uint8_t volumePercent)
{
	if (volumePercent == 0 || toneHz == 0)
	{
		assert0(rt_pwm_set(pwm_dev, PWM_CH_BEEP, 1000, 0));
		return;
	}
	uint32_t period = (1000 * 1000 * 1000 / toneHz);
	uint32_t pulse = (period / 10000) * 70 * volumePercent;
#if BEEP_INVERT
	pulse = period - pulse;
#endif
	_DEV_DEBUG("%dHz, %d%% ::: period=%lu, pulse=%lu", toneHz, volumePercent, period, pulse);
	assert0(rt_pwm_set(pwm_dev, PWM_CH_BEEP, period, pulse));
}

static void buzzer_play_delay(uint16_t toneHz, uint8_t volumePercent, uint32_t timeMs)
{
	buzzer_play(toneHz, volumePercent);
	rt_thread_mdelay(timeMs);
	buzzer_play(0, 0);
}

static void beep_thread_main(void *args)
{
	_DEV_DEBUG("beep thread started.");
	beep_state s;
	while (1)
	{
		if (rt_mq_recv(queue, &s, sizeof(beep_state), RT_WAITING_FOREVER) == RT_EOK)
		{
			buzzer_play_delay(s.toneHz, s.volumePercent, s.timeMs);
		}
	}
}

void beep_thread_init()
{
	assert0(rt_pwm_enable(pwm_dev, PWM_CH_BEEP));

	queue = rt_mq_create("beep_queue", sizeof(beep_state), BEEP_QUEUE, RT_IPC_FLAG_PRIO);
	assert(queue != NULL);

	thread = rt_thread_create("beep", beep_thread_main, NULL, 512, RT_THREAD_PRIORITY_MAX - 3, 2);
	assert(thread != NULL);
	rt_thread_startup(thread);
}

void beep_error()
{
	static rt_bool_t error_state = RT_FALSE;

	if (error_state)
		return;
	else
		error_state = RT_TRUE;

	return;

	rt_kputs("alert error!!!\n");
	if (thread != NULL)
		rt_thread_delete(thread);
	if (queue != NULL)
		rt_mq_delete(queue);
	buzzer_play(200, 10);
}
