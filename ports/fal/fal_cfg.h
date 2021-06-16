#pragma once

#include "storage.h"
#include <rt-thread-w60x/internal-flash.h>

/* flash device table */
#define FAL_FLASH_DEV_TABLE \
	{                       \
		&w60x_onchip,       \
	}

#define FAL_PART_TABLE                      \
	{                                       \
		W600_INTERNAL_FLASH_PARTITION_TABLE \
	}
