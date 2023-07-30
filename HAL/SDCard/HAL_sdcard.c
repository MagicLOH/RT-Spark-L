//
// Created by ds on 2023/7/27.
//

#include "HAL_sdcard.h"

#include <rtthread.h>

//#include <drv_lcd.h>

#define DBG_TAG "HAL_sdcard"
#define DBG_LVL DBG_LOG

#include <rtdbg.h>

#define LCD_MAX_NUM (32)

static char page_buf[LCD_MAX_NUM];


rt_err_t SD_GetFileInfos(const char *path, NovelInfo_t *novel)
{
    int fd = open(path, O_RDONLY);
    if (fd < 0)
    {
        rt_kprintf("sdcard open novel file failed!");
        return -RT_ERROR;
    }
    LOG_I("sdcard open novel file successfully.");

    // record novel path
    rt_strcpy(novel->path, path);
    // record novel name
    char *p = rt_strstr(path, ".txt");
    while (*p != '/') p--;
    rt_strcpy(novel->name, p + 1);
    // record novel size
    struct stat f_stat = {0};
    fstat(fd, &f_stat);
    novel->size = f_stat.st_size;
    // calculate novel pages
    novel->pages = novel->size / LCD_MAX_NUM + 1;
    // record origin offset
    novel->offset = 0; // offset = pages * max_len

//	ssize_t f_size = f_stat.st_size; // 获取文件总大小
//	ssize_t s = 0;
//	while (f_size != s)
//	{
//		s += read(fd, page_buf, sizeof(page_buf));
//		if (s < 0)
//		{
//			LOG_E("sdcard read data failed!");
//			return RT_ERROR;
//		}
//	}
//	LOG_I("Novel file read done. s = %ld", s);
    close(fd);
    return RT_EOK;
}

void SD_Init(void)
{
    LOG_I("Waiting for fs mount ok.");
    rt_thread_mdelay(1000);

    if (dfs_mount("sd0", "/sdcard", "elm", 0, 0) == RT_EOK)
    {
        LOG_I("SD card mount to '/sdcard'");
    }
    else
    {
        LOG_E("SD card mount to '/sdcard' failed!");
        return;
    }
}