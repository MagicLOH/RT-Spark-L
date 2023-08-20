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

    HAL::SD_Init(); // manually mount sd

    
    LOG_I("myHAL_Init done.");
    return 0;
}
INIT_APP_EXPORT(myHAL_Init);