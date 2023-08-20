#include "HAL.h"
#include "HAL_Def.h"

#include <rtthread.h>

#define DBG_TAG "HAL_sdcard"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

#include <dfs_file.h>
#include <unistd.h>

#define LCD_MAX_NUM (32)

static char page_buf[LCD_MAX_NUM];

rt_err_t SD_DirScan(const char *path)
{

    return RT_EOK;
}

rt_err_t SD_GetFileInfos(int argc, char *argv[])
{
    if (argc != 2)
    {
        LOG_E("Usage: %s [path/filename]", argv[0]);
        return RT_ERROR;
    }

    int fd = open(argv[1], O_RDONLY);
    if (fd < 0)
    {
        rt_kprintf("sdcard open novel file failed!\n");
        return -RT_ERROR;
    }
    LOG_I("sdcard open novel file successfully.");

    NovelInfo_t novel = {0};
    // record novel path
    rt_strcpy(novel.path, argv[1]);
    // record novel name
    char *p = rt_strstr(argv[1], ".");
    while (*p != '/') p--;
    rt_strcpy(novel.name, p + 1);
    // record novel size
    struct stat f_stat = {0};
    fstat(fd, &f_stat);
    novel.size = f_stat.st_size;
    // calculate novel pages
    novel.pages = novel.size / LCD_MAX_NUM + 1;
    // record origin offset
    novel.offset = 0; // offset = pages * max_len

    LOG_I("SD GOT INFO: PATH: %s | NAME: %s | SIZE:%ld | PAGES: %ld | OFFSET: %ld",
          novel.path, novel.name, novel.size, novel.pages, novel.offset);

    close(fd);
    return RT_EOK;
}
MSH_CMD_EXPORT(SD_GetFileInfos, sd_print_infos);

/**
 * @brief: manually mount sdcard on file system
 */
void HAL::SD_Init()
{
//	rt_thread_delay(100);
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