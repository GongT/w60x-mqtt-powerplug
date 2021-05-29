// place custom config here
#pragma once
#define EF_STR_ENV_VALUE_MAX_SIZE 0
#define FAL_PRINTF printf
#define APPLICATION_KIND "plug"
// #define CONFIG_INTERFACE_NETDEV (netdev_get_by_name("w0"))
#define CONFIG_INTERFACE_NETDEV (netdev_default)
// #define LWIP_HOOK_DHCP_APPEND_OPTIONS test_dhcp_set_options
#define FAL_PARTITION_STORAGE "eflash"
#define EF_STR_ENV_VALUE_MAX_SIZE 0

// ### flash settings
#include "wm_flash_map.h"
// ./packages/wm_libraries-latest/Include/Driver/wm_flash_map.h
#define INTERNAL_USER_START (0x80F0000UL - FLASH_BASE_ADDR)
#define INTERNAL_USER_END (0x80FBFFFUL - FLASH_BASE_ADDR)

#define UPDATE_IMAGE_START (0x8090000UL)
#define UPDATE_IMAGE_END (0x80EFFFFUL)

#define UPDATE_PARAM_START (0x80FC000UL)
// ### flash settings END
