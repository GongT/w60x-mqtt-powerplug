#define DBG_SECTION_NAME "IO:TEST"
#include "app.h"

#ifdef RT_USING_FINSH

#include <finsh.h>

static int get_pin(const char *name)
{
	int r = atoi(name);
	if (r > 0 || strcmp(name, "0") == 0)
	{
		return r;
	}
	// LOG_I("get_pin: 0 = %c", name[0]);
	if (name[0] == 'P' || name[0] == 'p')
	{
		name++;
	}
	int pn = atoi(name + 1);
	// LOG_I("get_pin: 0 = %c pn = %d", name[0], pn);
	if (name[0] == 'A' || name[0] == 'a')
	{
		// LOG_I("get_pin: A + %d", pn);
		switch (pn)
		{
		case 0:
			return PIN_NUM_A00;
		case 1:
			return PIN_NUM_A01;
		case 4:
			return PIN_NUM_A04;
		case 5:
			return PIN_NUM_A05;
		}
		// LOG_I("get_pin: A invalid");
	}
	else if (name[0] == 'B' || name[0] == 'b')
	{
		// LOG_I("get_pin: B + %d", pn);
		switch (pn)
		{
		case 13:
			return PIN_NUM_B13;
		case 14:
			return PIN_NUM_B14;
		case 15:
			return PIN_NUM_B15;
		case 16:
			return PIN_NUM_B16;
		case 17:
			return PIN_NUM_B17;
		case 18:
			return PIN_NUM_B18;
		case 6:
			return PIN_NUM_B06;
		case 7:
			return PIN_NUM_B07;
		case 8:
			return PIN_NUM_B08;
		case 9:
			return PIN_NUM_B09;
		case 10:
			return PIN_NUM_B10;
		case 11:
			return PIN_NUM_B11;
		case 12:
			return PIN_NUM_B12;
		}
		// LOG_I("get_pin: B invalid");
	}
	// LOG_I("get_pin: return -1");

	LOG_E("invalid pin: %s", name);
	return -1;
}

static long usage(const char *msg)
{
	printf(msg);
	printf("\n");
	return 1;
}

static long gpio(int argc, char **argv)
{
#define ARG_CMP(num, val) ((argc >= num + 1) && str_eq(argv[num], val))
	if (ARG_CMP(1, "read") || ARG_CMP(1, "r"))
	{
		if (argc != 3)
			return usage("Usage: gpio read PA0");
		int pin = get_pin(argv[2]);
		if (pin < 0)
			return 1;
		int ret = rt_pin_read(pin);
		LOG_I("read pin %d value = %d", pin, ret);
	}
	else if (ARG_CMP(1, "write") || ARG_CMP(1, "w"))
	{
		if (argc != 4)
			return usage("Usage: gpio write PA0 0/1/h/l");
		int pin = get_pin(argv[2]);
		if (pin < 0)
			return 1;
		int value = atoi(argv[3]);
		LOG_I("write pin %d value = %d", pin, value);
		rt_pin_write(pin, value);
	}
	else if (ARG_CMP(1, "mode"))
	{
		ARG_CMP(3, "in");
		int pin = get_pin(argv[2]);
		if (pin < 0)
			return 1;
		if (ARG_CMP(3, "in") || ARG_CMP(3, "i"))
		{
			LOG_I("set pin %d mode %d", pin, PIN_MODE_INPUT_PULLUP);
			rt_pin_mode(pin, PIN_MODE_INPUT_PULLUP);
		}
		else if (ARG_CMP(3, "out") || ARG_CMP(3, "o"))
		{
			if (argc != 4)
				return usage("Usage: gpio mode PA0 o");

			LOG_I("set pin %d mode %d", pin, PIN_MODE_OUTPUT);
			rt_pin_mode(pin, PIN_MODE_OUTPUT);
		}
		else
		{
			return usage("Usage: gpio mode PA0 i/o");
		}
	}
	else if (ARG_CMP(1, "cfg"))
	{
		if (argc == 3)
		{
			int pin = get_pin(argv[2]);
			if (pin < 0)
				return 1;

			int v = tls_io_cfg_get(pin);
			LOG_I("tls io cfg: pin=%d   cfg=%d", pin, v);
		}
		else if (argc == 4)
		{
			int pin = get_pin(argv[2]);
			if (pin < 0)
				return 1;
			int v = atoi(argv[3]);

			LOG_I("tls io cfg: pin=%d, set to: %d", pin, v);
			tls_io_cfg_set(pin, v);
			LOG_I("    result: %d", tls_io_cfg_get(pin));
		}
		else
		{
			return usage("Usage: gpio cfg <pin>");
		}
	}
	else
	{
		return usage("Usage: gpio read/r/write/w/mod/cfg <pin> [value]");
	}
	return 0;
}

MSH_CMD_EXPORT(gpio, gpio read write);

#endif
