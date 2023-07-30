//
// Created by ds on 2023/7/27.
//

#include "HAL.h"
#include "HAL_button.h"
#include "HAL_LCD.h"
//#include "HAL_sdcard.h"

#define DBG_TAG "HAL"
#define DBG_LVL DBG_LOG

#include <rtdbg.h>
#include <rtthread.h>

int myHAL_Init(void)
{
    MultiButton_Init();

//    lcd_show_string(0, 0, 16, "fuckyou");

//    SD_Init();

    LOG_I("HAL_Init done.");
}
INIT_ENV_EXPORT(myHAL_Init);
