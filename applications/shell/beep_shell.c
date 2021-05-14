#define DBG_SECTION_NAME "sh:beep"
#include "shell.h"

extern struct rt_device_pwm *pwm_dev;

static rt_bool_t parse_state(beep_state_t store, int argc, char **argv)
{
	if (argc != 4)
	{
		rt_kprintf("%s tone<Hz> volume<%%> time<ms>\n", argv[0]);
		return RT_FALSE;
	}

	uint32_t toneHz = atol(argv[1]);
	uint32_t volumePercent = atol(argv[2]);
	uint32_t timeMs = atol(argv[3]);

	store->toneHz = toneHz;
	store->volumePercent = volumePercent;
	store->timeMs = timeMs;

	return RT_TRUE;
}

static long beep(int argc, char **argv)
{
	static beep_state s;
	if (!parse_state(&s, argc, argv))
		return 1;
	buzzer_beep(s.toneHz, s.volumePercent, s.timeMs);
	return 0;
}
MSH_CMD_EXPORT_ALIAS(beep, beep, beep once);

#define TEST_QUEUE_SIZE 32
static beep_state queue[TEST_QUEUE_SIZE];
static int current = -1;
static long beep_queue(int argc, char **argv)
{
	int c = current + 1;

	if (c == TEST_QUEUE_SIZE)
		RETURN_LOG_E("test queue is full");
	if (!parse_state(&queue[c], argc, argv))
		return 1;

	current = c;
	return 0;
}
MSH_CMD_EXPORT_ALIAS(beep_queue, beep_queue, add beep to queue);

static long beep_queue_emit()
{
	for (int i = 0; i <= current; i++)
	{
		buzzer_beep(queue[i].toneHz, queue[i].volumePercent, queue[i].timeMs);
	}
	current = -1;
	return 0;
}
MSH_CMD_EXPORT_ALIAS(beep_queue_emit, beep_queue_emit, emit beep_queue);
