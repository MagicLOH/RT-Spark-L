#ifndef __HAL_DEF_H
#define __HAL_DEF_H

#include <rtdef.h>
#include "multi_button.h"

/* Button */
typedef enum
{
	BTN_UP_ID = 1,
	BTN_DOWN_ID,
	BTN_LEFT_ID,
	BTN_RIGHT_ID
} btn_id_t;

typedef struct
{
	struct Button *btn_handler;
	rt_base_t btn_pin;
	rt_uint8_t btn_num : 3; 
} btn_context_t;

/* SD */
typedef struct
{
	char path[64];
	char name[32];
	rt_off_t size;
	rt_off_t pages;
	rt_off_t offset;
} File_Info_t;

/* LCD */
#define LCD_WIDTH (240)
#define LCD_HEIGHT (240)

#define CHN_FONT_16x16 (16)
#define CHN_FONT_24x24 (24)
#define CHN_FONT_32x32 (32)

// #define MAX_CHARS_DISP 30
#define MAX_LINE_SIZE ((LCD_WIDTH) / (CHN_FONT_24x24 / 2))
#define MAX_COLUMN_SIZE (((LCD_HEIGHT - CHN_FONT_24x24)) / (CHN_FONT_24x24))

#define MAX_CHARS_DISP ((MAX_LINE_SIZE) * (MAX_COLUMN_SIZE))

#endif
