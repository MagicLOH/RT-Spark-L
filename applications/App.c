//
// Created by ds on 2023/7/27.
//

#include "App.h"
#include "HAL.h"

#include <rtthread.h>

#define DBG_TAG "App"
#define DBG_LVL DBG_LOG

#include <rtdbg.h>

#define NOVEL_PATH "/sdcard/1.txt"

int App_Init(void)
{
    rt_err_t err = RT_EOK;
//    err = SD_GetFileInfos(NOVEL_PATH);
//    if (RT_EOK != err)
//    {
//        LOG_E("SD GotInfos failed! err = %ld", rt_get_errno());
//        return -err;
//    }


    LOG_I("App_Init done.");
    return err;
}
