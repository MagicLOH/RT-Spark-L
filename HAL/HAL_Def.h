#ifndef __HAL_DEF_H
#define __HAL_DEF_H

#include <rtdef.h>

/* Button */
typedef enum
{
    btnUp_id = 1,
    btnDown_id,
    btnLeft_id,
    btnRight_id,
} Button_ID_t;

/* SD */
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

#endif