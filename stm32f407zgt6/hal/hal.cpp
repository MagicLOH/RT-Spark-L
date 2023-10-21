#include "hal.h"

#define DBG_TAG         "hal"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

static int hal_init(void)
{
#ifdef BSP_USING_ONCHIP_RTC
	hal::RTC_Init();
#endif

#ifdef OFFLINE_PKG_USING_MULTIBUTTON
	hal::btn_init();
#endif

#ifdef BSP_USING_FS
#if !defined(BSP_USING_FS_AUTO_MOUNT) && defined(BSP_USING_SDCARD_FATFS)
	hal::sd_mount_fs();
#endif

#if !defined(BSP_USING_FLASH_FS_AUTO_MOUNT) && defined(BSP_USING_FLASH_FATFS)
	hal::spi_flash_mount_fs();
#endif
#endif

	LOG_I("hal_init done.");
	return 0;
}
INIT_APP_EXPORT(hal_init);
