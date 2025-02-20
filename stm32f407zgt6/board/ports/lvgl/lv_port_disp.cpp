/*********************
 *      INCLUDES
 *********************/
#include "lv_conf.h"
#include "lv_port.h"
#include "lvgl.h"

extern "C" {
#include "drv_ST7789.h"
}


/*********************
 *      DEFINES
 *********************/
#ifndef MY_DISP_HOR_RES
#warning Please define or replace the macro MY_DISP_HOR_RES with the actual screen width, default value 320 is used for now.
#define MY_DISP_HOR_RES    240
#endif

#ifndef MY_DISP_VER_RES
#warning Please define or replace the macro MY_DISP_HOR_RES with the actual screen height, default value 240 is used for now.
#define MY_DISP_VER_RES    240
#endif

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void disp_flush(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p);
//static void gpu_fill(lv_disp_drv_t * disp_drv, lv_color_t * dest_buf, lv_coord_t dest_width,
//        const lv_area_t * fill_area, lv_color_t color);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void lv_port_disp_init(void)
{
	/*-------------------------
	 * Initialize your display
	 * -----------------------*/

	/*-----------------------------
	 * Create a buffer for drawing
	 *----------------------------*/

	/**
	 * LVGL requires a buffer where it internally draws the widgets.
	 * Later this buffer will passed to your display driver's `flush_cb` to copy its content to your display.
	 * The buffer has to be greater than 1 display row
	 *
	 * There are 3 buffering configurations:
	 * 1. Create ONE buffer:
	 *      LVGL will draw the display's content here and writes it to your display
	 *
	 * 2. Create TWO buffer:
	 *      LVGL will draw the display's content to a buffer and writes it your display.
	 *      You should use DMA to write the buffer's content to the display.
	 *      It will enable LVGL to draw the next part of the screen to the other buffer while
	 *      the data is being sent form the first buffer. It makes rendering and flushing parallel.
	 *
	 * 3. Double buffering
	 *      Set 2 screens sized buffers and set disp_drv.full_refresh = 1.
	 *      This way LVGL will always provide the whole rendered screen in `flush_cb`
	 *      and you only need to change the frame buffer's address.
	 */

	/* Example for 1) */
	static lv_disp_draw_buf_t draw_buf_dsc_1;

	/* GCC or AC6 */
#if defined ( __GNUC__ ) || defined ( __clang__ )
	static lv_color_t buf_1[MY_DISP_HOR_RES * MY_DISP_HOR_RES /
	                        2] __attribute__((section(".LVGLccm"))); /* A buffer for 10 rows */
	/* MDK AC5 */
#elif defined ( __CC_ARM )
	__attribute__((at(0x10000000))) lv_color_t buf_1[LCD_H * LCD_W / 2];
#endif

	/*Initialize the display buffer*/
	lv_disp_draw_buf_init(&draw_buf_dsc_1, buf_1, NULL,
	                      MY_DISP_HOR_RES * MY_DISP_HOR_RES / 2);

	/*-----------------------------------
	 * Register the display in LVGL
	 *----------------------------------*/
	static lv_disp_drv_t disp_drv;    /*Descriptor of a display driver*/
	lv_disp_drv_init(&disp_drv);    /*Basic initialization*/

	/*Set up the functions to access to your display*/

	/*Set the resolution of the display*/
	disp_drv.hor_res = MY_DISP_HOR_RES;
	disp_drv.ver_res = MY_DISP_VER_RES;

	/*Used to copy the buffer's content to the display*/
	disp_drv.flush_cb = disp_flush;

	/*Set a display buffer*/
	disp_drv.draw_buf = &draw_buf_dsc_1;

	/*Required for Example 3)*/
	//disp_drv.full_refresh = 1;

	/* Fill a memory array with a color if you have GPU.
	 * Note that, in lv_conf.h you can enable GPUs that has built-in support in LVGL.
	 * But if you have a different GPU you can use with this callback.*/
	//disp_drv.gpu_fill_cb = gpu_fill;

	/*Finally register the driver*/
	lv_disp_drv_register(&disp_drv);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

static void disp_flush(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p)
{
	lcd_fill_array(area->x1, area->y1, area->x2, area->y2, color_p);

	lv_disp_flush_ready(disp_drv);
}

