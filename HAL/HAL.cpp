#include "HAL.h"

#define DBG_TAG "HAL"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

void HAL::HAL_Update(void)
{
    RTC_Update();
    Button_Update();
}

/*=====================
  	Export HAL Init
 *=====================*/
static int myHAL_Init(void)
{
    HAL::RTC_Init();

    HAL::Button_Init();

#ifndef BSP_USING_FS_AUTO_MOUNT
    HAL::SD_MountFS();
#endif

#ifndef BSP_USING_FLASH_FS_AUTO_MOUNT
    HAL::SPI_Flash_MountFS();
#endif

//    char buf[128] = {0};
//    HAL::SPI_Flash_Read("font", buf, 0x7F4A0, 32);
//    for (int i = 0; i < 32; i++)
//    {
//        rt_kprintf("0x%02X ", buf[i]);
//    }
//    rt_kprintf("\n");

    LOG_I("myHAL_Init done.");
    return 0;
}
INIT_APP_EXPORT(myHAL_Init);
