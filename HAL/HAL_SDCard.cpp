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

static rt_err_t SD_GetFileInfos(int argc, char *argv[])
{
    if (argc != 2)
    {
        LOG_W("Usage: %s [path/filename]", argv[0]);
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
MSH_CMD_EXPORT_ALIAS(SD_GetFileInfos, sd_print_infos, print one file infos);

static rt_err_t SD_Read(int argc, char *argv[])
{
    if (argc != 2)
    {
        LOG_W("Usage: %s <path/filename>", argv[0]);
        return RT_ERROR;
    }

    int fd = open(argv[1], O_RDONLY);
    if (fd < 0)
    {
        rt_kprintf("sdcard open file failed!\n");
        return -RT_ERROR;
    }
//    LOG_I("sdcard open novel file successfully.");

    rt_kprintf("================READ================\n");
    char buf[128] = {0};
    while (1)
    {
        ssize_t cnt = read(fd, buf, sizeof(buf) - 1);
        if (cnt < 0)
        {
            LOG_E("read file failed!");
            return -RT_ERROR;
        }
        else if (cnt != sizeof(buf) - 1)
        {
            buf[cnt] = '\0';
//            rt_kprintf("read count = (%d)\n", cnt);
            rt_kprintf("%s\n", buf);
            rt_kprintf("====================================\n");
            break;
        }
        else
        {
            buf[cnt] = '\0';
//            rt_kprintf("read count = (%d)\n", cnt);
            rt_kprintf("%s\n", buf);
        }
    }

    close(fd);
    return RT_EOK;
}
MSH_CMD_EXPORT_ALIAS(SD_Read, sd_read, simply read a file);

static rt_err_t SD_Write(int argc, char *argv[])
{
    if (argc != 3)
    {
        LOG_W("Usage: %s <dest> <src>", argv[0]);
        return -RT_ERROR;
    }
    int fd = open(argv[1], O_RDWR | O_CREAT);
    if (fd < 0)
    {
        LOG_E("open fill failed, please check path is exist!");
        return -RT_ERROR;
    }

    char *p = argv[2];
    ssize_t w_size = write(fd, p, strlen(p) + 1); // remain one character for '\0'
    if (w_size < 0)
    {
        LOG_E("write failed!");
        return -RT_ERROR;
    }
    rt_kprintf("write count = %d\n", w_size);

    close(fd);
    return RT_EOK;
}
MSH_CMD_EXPORT_ALIAS(SD_Write, sd_write, simply write to a file);

/**
 * @brief: manually mount sdcard on file system
 */
void HAL::SD_MountFS()
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

void HAL::SD_FontLib_Update()
{

}