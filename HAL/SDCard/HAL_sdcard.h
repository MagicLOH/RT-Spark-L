//
// Created by ds on 2023/7/27.
//

#ifndef _HAL_SDCARD_H_
#define _HAL_SDCARD_H_

#include <dfs_posix.h>

typedef struct NovelInfo
{
    char path[64];
    char name[32];
    rt_off_t size;
    rt_off_t pages;
    rt_off_t offset;
} NovelInfo_t;
typedef struct Novel
{
    char path[32];
    char name[16];
    rt_off_t size;
    rt_off_t current_page;
    struct Novel *next;
} Novel_t;

void SD_Init(void);
rt_err_t SD_GetFileInfos(const char *path);

#endif //_HAL_SDCARD_H_
