#pragma once

#include "storage.h"

extern struct fal_flash_dev w60x_onchip;

/* flash device table */
#define FAL_FLASH_DEV_TABLE \
	{                       \
		&w60x_onchip,       \
	}

#define KEY_VALUE_PARTITION \
	{FAL_PART_MAGIC_WROD, FAL_PARTITION_STORAGE, "w60x_onchip", INTERNAL_USER_START, INTERNAL_USER_END - INTERNAL_USER_START, 0},

#define FAL_PART_TABLE      \
	{                       \
		KEY_VALUE_PARTITION \
	}
