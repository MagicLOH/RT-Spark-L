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
    ssize_t size;
    ssize_t pages;
    ssize_t offset;
} NovelInfo_t;

void SD_Init(void);
rt_err_t SD_GetFileInfos(const char *path, NovelInfo_t *novel);

#endif //_HAL_SDCARD_H_
