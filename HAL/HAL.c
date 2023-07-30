/**
 * Hardware Abstract layer
 */
#include "HAL.h"


#define DBG_TAG "HAL"
#define DBG_LVL DBG_LOG

#include <rtdbg.h>


int HAL_In1t(void)
{
    RTC_Init();
    Button_Init();
    SD_Init();

    LOG_I("HAL_Init done.");
}
INIT_APP_EXPORT(HAL_In1t);