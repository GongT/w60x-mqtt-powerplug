#include <easyflash.h>
#include <rtthread.h>
#include <fal.h>
#include "ef_cfg.h"
#include "helpers.h"

static const struct fal_partition *part = NULL;

rt_bool_t easy_flash_extra_init()
{
	KPRINTF_DIM("EasyFlash port: fal");
	fal_init();
	part = fal_partition_find(FAL_PARTITION_STORAGE);
	return part != NULL;
}

EfErrCode ef_port_read(uint32_t addr, uint32_t *buf, size_t size)
{
	assert(addr >= 0 && addr < ENV_AREA_SIZE);
	// KPRINTF_DIM("fal_partition_read(0x%X, %d)", addr, size);
	if (fal_partition_read(part, addr, (void *)buf, size) > 0)
		return EF_READ_ERR;
	return EF_NO_ERR;
}
EfErrCode ef_port_erase(uint32_t addr, size_t size)
{
	assert(addr >= 0 && addr < ENV_AREA_SIZE);
	// KPRINTF_DIM("fal_partition_erase(0x%X, %d)", addr, size);
	if (fal_partition_erase(part, addr, size) < 0)
		return EF_ERASE_ERR;
	return EF_NO_ERR;
}
EfErrCode ef_port_write(uint32_t addr, const uint32_t *buf, size_t size)
{
	assert(addr >= 0 && addr < ENV_AREA_SIZE);
	// KPRINTF_DIM("fal_partition_write(0x%X, ..., %d)", addr, size);
	if (fal_partition_write(part, addr, (void *)buf, size) < 0)
		return EF_WRITE_ERR;
	return EF_NO_ERR;
}
