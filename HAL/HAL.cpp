#include "HAL.h"

#define DBG_TAG "HAL"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

void HAL::HAL_Update(void)
{
    RTC_Update();
    Button_Update();
}

/**====================
 * 		Hal init
 *=====================*/
static int myHAL_Init(void)
{
    HAL::RTC_Init();

    HAL::Button_Init();

#ifndef BSP_USING_FS_AUTO_MOUNT
    HAL::SD_MountFS();
#endif

#ifndef BSP_USING_FLASH_FS_AUTO_MOUNT
    HAL::SPIFlash_MountFS();
#endif

    LOG_I("myHAL_Init done.");
    return 0;
}
INIT_APP_EXPORT(myHAL_Init);