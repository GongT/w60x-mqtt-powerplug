#include "shell.h"

#define USAGE_RETURN(MSG, ...)                                                                   \
	do {                                                                                         \
		if (strlen(MSG))                                                                         \
			rt_kprintf(MSG "\n", ##__VA_ARGS__);                                                 \
		rt_kprintf("led use red/green <current: %s>\n", selection == LED_RED ? "red" : "green"); \
		rt_kputs("led f/fade cycle(ms)\n");                                                      \
		rt_kputs("led b/blink cycle(ms)\n");                                                     \
		rt_kputs("led s/static level(0-100)\n");                                                 \
		rt_kputs("led on/off\n");                                                                \
		return 1;                                                                                \
	} while (0)

static int selection = LED_RED;

static int on_off(const char *arg) {
	if (str_eq(arg, "on"))
		led_on(selection);
	else if (str_eq(arg, "off"))
		led_off(selection);
	else
		return 2;
	return 0;
}
static int use(const char *arg) {
	if (str_prefix("red", arg)) {
		rt_kprintf("selected red led\n");
		selection = LED_RED;
	} else if (str_prefix("green", arg)) {
		rt_kprintf("selected green led\n");
		selection = LED_GREEN;
	} else
		RETURN_WITH_ERR("invalid selection.");
	return 0;
}

static int set_fade(int value) {
	if (value < 1000 || value > 10000)
		RETURN_WITH_ERR("cycle only allow 1000(1s)~10000(10s)");
	led_fade(selection, value);
	return 0;
}
static int set_blink(int value) {
	if (value < 100 || value > 2000)
		RETURN_WITH_ERR("cycle only allow 100(0.1s)~2(2s)");
	led_blink(selection, value);
	return 0;
}
static int set_static(int value) {
	if (value < 0 || value > 100)
		RETURN_WITH_ERR("level only allow 0~100");
	led_static(selection, value);
	return 0;
}

static long led_commands(int argc, char **argv) {
	int r = 2;
	if (argc == 2) {
		r = on_off(argv[1]);
		if (r != 0)
			USAGE_RETURN("invalid command: %s", argv[2]);
	} else if (argc == 3) {
		if (str_prefix("use", argv[1]))
			r = use(argv[2]);
		else if (str_prefix("fade", argv[1]))
			r = set_fade(atoi(argv[2]));
		else if (str_prefix("blink", argv[1]))
			r = set_blink(atoi(argv[2]));
		else if (str_prefix("static", argv[1]))
			r = set_static(atoi(argv[2]));
		else
			USAGE_RETURN("invalid command: %s", argv[2]);
	}

	if (r == 2)
		USAGE_RETURN("Usage:");

	return r;
}
DEFINE_CMD(led_commands, led, set led status);
