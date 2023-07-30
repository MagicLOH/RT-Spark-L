//
// Created by ds on 2023/7/27.
//

#include "HAL.h"
#include "HAL_button.h"
#include "HAL_lcd.h"
#include "HAL_rtc.h"
#include "HAL_sdcard.h"

#define DBG_TAG "HAL"
#define DBG_LVL DBG_LOG

#include <rtdbg.h>
#include <rtthread.h>

int HAL_In1t(void)
{
    RTC_Init();
    MultiButton_Init();
    SD_Init();

//    lcd_show_string(0, 0, 16, "fuck you");

    LOG_I("HAL_Init done.");
}
INIT_APP_EXPORT(HAL_In1t);