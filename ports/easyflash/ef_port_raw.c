#include <easyflash.h>
#include <drv_flash.h>
#include <wm_flash_map.h>
#include "ef_cfg.h"
#include "helpers.h"

rt_bool_t easy_flash_extra_init()
{
	KPRINTF_DIM("EasyFlash port: raw");
	return RT_TRUE;
}

EfErrCode ef_port_read(uint32_t addr, uint32_t *buf, size_t size)
{
	assert(addr >= 0 && addr < ENV_AREA_SIZE);
	addr += USER_AREA_OFFSET;
	// KPRINTF_DIM("wm_flash_read(0x%X, %d)", addr, size);
	if (wm_flash_read(addr, (void *)buf, size) > 0)
		return EF_READ_ERR;
	return EF_NO_ERR;
}
EfErrCode ef_port_erase(uint32_t addr, size_t size)
{
	assert(addr >= 0 && addr < ENV_AREA_SIZE);
	addr += USER_AREA_OFFSET;
	// KPRINTF_DIM("wm_flash_erase(0x%X, %d)", addr, size);
	if (wm_flash_erase(addr, size) < 0)
		return EF_ERASE_ERR;
	return EF_NO_ERR;
}
EfErrCode ef_port_write(uint32_t addr, const uint32_t *buf, size_t size)
{
	assert(addr >= 0 && addr < ENV_AREA_SIZE);
	addr += USER_AREA_OFFSET;
	// KPRINTF_DIM("wm_flash_write(0x%X, ..., %d)", addr, size);
	if (wm_flash_write(addr, (void *)buf, size) < 0)
		return EF_WRITE_ERR;

	return EF_NO_ERR;
}
