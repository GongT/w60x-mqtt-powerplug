#pragma once

#include <rtconfig.h>

#include "../../packages/EasyFlash-v4.1.0/inc/ef_cfg.h"

#include <rt-thread-w60x/internal-flash.h>

#define USER_AREA_OFFSET (USER_ADDR_START - FLASH_BASE_ADDR)

#undef ENV_AREA_SIZE
#define ENV_AREA_SIZE ((USER_AREA_LEN - EF_START_ADDR) / EF_ERASE_MIN_SIZE * EF_ERASE_MIN_SIZE)
