//
// Created by ds on 2023/7/27.
//

#include "App.h"
#include "HAL.h"

#include <rtthread.h>

#define DBG_TAG "App"
#define DBG_LVL DBG_LOG

#include <rtdbg.h>


int App_Init(void)
{
    rt_err_t err = RT_EOK;
    NovelInfo_t novel = {0};
    if (RT_EOK == SD_GetFileInfos("/sdcard/1.txt", &novel))
    {
        LOG_I("PATH: %s\tNAME: %s\tSIZE:%ld\tPAGES: %ld\tOFFSET: %ld",
              novel.path, novel.name, novel.size, novel.pages, novel.offset);
    }


    LOG_I("App_Init done.");
    return err;
}
