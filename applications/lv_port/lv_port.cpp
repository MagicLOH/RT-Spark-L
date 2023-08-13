#include <lvgl.h>

#include "HAL.h"
#include "App.h"
#define DBG_TAG "lv_port"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

#ifndef PKG_LVGL_THREAD_STACK_SIZE
#define PKG_LVGL_THREAD_STACK_SIZE 4096
#endif /* PKG_LVGL_THREAD_STACK_SIZE */

#ifndef PKG_LVGL_THREAD_PRIO
#define PKG_LVGL_THREAD_PRIO (RT_THREAD_PRIORITY_MAX*2/3)
#endif /* PKG_LVGL_THREAD_PRIO */

#include "lv_port_disp.h"
#include "lv_port_indev.h"
#include "lv_port_fs.h"

static struct rt_thread lvgl_thread;

#ifdef rt_align
rt_align(RT_ALIGN_SIZE)
#else
ALIGN(RT_ALIGN_SIZE)
#endif
static rt_uint8_t lvgl_thread_stack[PKG_LVGL_THREAD_STACK_SIZE];

#if LV_USE_LOG
static void lv_rt_log(const char *buf)
{
	LOG_I(buf);
}
#endif /* LV_USE_LOG */

void lv_user_gui_init(void)
{
}

static void lvgl_thread_entry(void *parameter)
{
#if LV_USE_LOG
	lv_log_register_print_cb(lv_rt_log);
#endif /* LV_USE_LOG */
	lv_init();
	lv_port_disp_init();
	lv_port_indev_init();
	lv_port_fs_init();
	App_Init();

	/* handle the tasks of LVGL */
	while (1)
	{
		HAL_Update();
		lv_task_handler();
		rt_thread_mdelay(LV_DISP_DEF_REFR_PERIOD);
	}
}

static int lvgl_thread_init(void)
{
	rt_err_t err;

	err = rt_thread_init(&lvgl_thread, "LVGL", lvgl_thread_entry, RT_NULL,
						 &lvgl_thread_stack[0], sizeof(lvgl_thread_stack), PKG_LVGL_THREAD_PRIO, 0);
	if (err != RT_EOK)
	{
		LOG_E("Failed to create LVGL thread");
		return -1;
	}
	rt_thread_startup(&lvgl_thread);
	LOG_I("lvgl_thread_init ok.");

	return 0;
}
INIT_APP_EXPORT(lvgl_thread_init);