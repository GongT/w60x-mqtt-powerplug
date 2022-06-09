#include "app.h"
#include <rtthread.h>

static rt_thread_t keepalive = NULL;

static void keep_alive() {
	static time_t now;
	static char buf[sizeof "1991-12-25T19:38:00Z"];

	while (1) {
		rt_thread_mdelay(10000);

		if (!is_mqtt_init()) {
			continue;
		}

		time(&now);
		strftime(buf, sizeof buf, "%FT%TZ", gmtime(&now));
		main_event_queue(REPORT_KEEPALIVE, buf, RT_FALSE);
	}
}
void start_keepalive_thread() {
	keepalive = rt_thread_create("keepalive", keep_alive, NULL, 10240, RT_THREAD_PRIORITY_MAX - 2, 5);
	rt_thread_startup(keepalive);
}
