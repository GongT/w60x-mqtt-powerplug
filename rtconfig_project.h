// place custom config here
#pragma once
#define EF_STR_ENV_VALUE_MAX_SIZE 0
#define FAL_PRINTF printf
#define APPLICATION_KIND "plug"
#define CONFIG_INTERFACE_NETDEV (netdev_get_by_name("w0"))
// #define CONFIG_INTERFACE_NETDEV (netdev_default)
// #define LWIP_HOOK_DHCP_APPEND_OPTIONS test_dhcp_set_options
#define FAL_PARTITION_STORAGE "eflash"
#define FAL_PARTITION_UPDATE_IMAGE "update"
#define EF_STR_ENV_VALUE_MAX_SIZE 0

#define MBEDTLS_ERR_PLATFORM_FEATURE_UNSUPPORTED -0xFF00
#define MBEDTLS_CONFIG_FILE "mbedtls_config.h"
#define rt_hwcrypto_dev_dufault rt_hwcrypto_dev_default
#define KAWAII_MQTT_LOG_TAR 1U
#define BOOT_CLEAR_SCREEN
