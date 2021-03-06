#include "app.h"
#include "storage.h"
#include "init.h"
#include <gongt/config_tool.h>

#include <easyflash.h>
#include <rtthread.h>
#include <stdlib.h>
#include <stdio.h>

DEFINE_CONFIG_NAMES(
	STORE_KEY_WIFI_NAME, STORE_KEY_WIFI_PASS, STORE_KEY_MQTT_DEV_TITLE, STORE_KEY_MQTT_SERVER_URI, STORE_KEY_MQTT_USER, STORE_KEY_MQTT_PASS, );

size_t get_storage_size(const char *key) {
	size_t r;
	if (ef_get_env_blob(key, NULL, SIZE_MAX, &r) == 0) {
		return 0;
	}
	return r;
}

rt_err_t save_config_item(const char *config_name, const char *value) {
	if (ef_set_and_save_env(config_name, value) == EF_NO_ERR) {
		return RT_EOK;
	} else {
		return RT_ERROR;
	}
}

void test_env(void) {
	uint32_t i_boot_times = 0;
	char c_boot_times[12] = {};

	if (ef_get_env_blob("boot_times", c_boot_times, sizeof c_boot_times, NULL) == 0) {
		i_boot_times = 0;
	} else {
		i_boot_times = atol(c_boot_times);
	}
	i_boot_times++;

	sprintf(c_boot_times, "%ld", i_boot_times);
	ef_set_env("boot_times", c_boot_times);

	ef_print_env();
}

rt_bool_t is_update_mode() {
	char c_update_mode[2] = "0";
	int i_update_mode = 0;
	if (ef_get_env_blob("update_mode", c_update_mode, sizeof c_update_mode, NULL) == 0) {
		return RT_FALSE;
	}

	i_update_mode = atol(c_update_mode);
	if (i_update_mode) {
		ef_set_and_save_env("update_mode", "0");
	}
	return i_update_mode ? RT_TRUE : RT_FALSE;
}
void set_update_mode() {
	ef_set_and_save_env("update_mode", "1");
}
