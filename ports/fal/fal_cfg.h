#pragma once

#include "storage.h"
#include <rt-thread-w60x/internal-flash.h>

extern struct fal_flash_dev w60x_onchip;

/* flash device table */
#define FAL_FLASH_DEV_TABLE \
	{                       \
		&w60x_onchip,       \
	}

#define FAL_PART(name, start, size)                                                \
	{                                                                              \
		FAL_PART_MAGIC_WROD, name, "w60x_onchip", start - FLASH_BASE_ADDR, size, 0 \
	}

#define PAGE(name, start) FAL_PART(name, start, INSIDE_FLS_PAGE_SIZE)
#define SECTOR(name, start) FAL_PART(name, start, INSIDE_FLS_SECTOR_SIZE)

#define FAL_PART_TABLE                                                                    \
	{                                                                                     \
		SECTOR("param_phy", TLS_PHY_PARAM_ADDR),                                          \
			SECTOR("param_flash", TLS_QFLASH_PARAM_ADDR),                                 \
			PAGE("secboot_hdr", SECBOOT_HEADER_ADDR),                                     \
			FAL_PART("secboot", SECBOOT_ADDR, SECBOOT_AREA_LEN),                          \
			PAGE("code_header", CODE_RUN_HEADER_ADDR),                                    \
			FAL_PART("code", CODE_RUN_START_ADDR, CODE_RUN_AREA_LEN),                     \
			FAL_PART(FAL_PARTITION_UPDATE_IMAGE, CODE_UPD_START_ADDR, CODE_UPD_AREA_LEN), \
			FAL_PART(FAL_PARTITION_STORAGE, USER_ADDR_START, USER_AREA_LEN),              \
			PAGE(FAL_PARTITION_UPDATE_IMAGE "_header", CODE_UPD_HEADER_ADDR),             \
			SECTOR("param_1", TLS_FLASH_PARAM1_ADDR),                                     \
			SECTOR("param_2", TLS_FLASH_PARAM2_ADDR),                                     \
			SECTOR("param_res", TLS_FLASH_PARAM_RESTORE_ADDR)                             \
	}
