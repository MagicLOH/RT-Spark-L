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
typedef struct Novel_Info
{
    char path[64];
    char name[32];
    rt_off_t size;
    rt_off_t pages;
    rt_off_t offset;
} NovelInfo_t;

/* LCD */
typedef enum
{
    CHN_FONT_16x16 = 16,
    CHN_FONT_24x24 = 24,
    CHN_FONT_32x32 = 32,
} LCD_FontSize_t;

#endif