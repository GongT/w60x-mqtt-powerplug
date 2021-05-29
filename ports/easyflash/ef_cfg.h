#pragma once

#include <rtconfig.h>

#include "../../packages/EasyFlash-v4.1.0/inc/ef_cfg.h"

#undef ENV_AREA_SIZE
#define ENV_AREA_SIZE ((INTERNAL_USER_END - INTERNAL_USER_START - EF_START_ADDR) / EF_ERASE_MIN_SIZE * EF_ERASE_MIN_SIZE)
