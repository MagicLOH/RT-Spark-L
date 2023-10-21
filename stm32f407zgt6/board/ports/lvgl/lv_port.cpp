#include "lv_conf.h"
#include "lv_port.h"

#ifdef OFFLINE_PKG_LVGL_USING_DEMOS
#include "lv_demos.h"
#endif

void lv_user_gui_init()
{
	/* display demo; you may replace with your LVGL application at here */
//    extern lv_demo_calendar();
//    lv_demo_calendar();

#if LV_USE_DEMO_MUSIC
	lv_demo_music();
#endif

#if LV_USE_DEMO_BENCHMARK
	lv_demo_benchmark();
#endif

#if LV_USE_DEMO_WIDGETS
	lv_demo_widgets();
#endif
}

void lv_port_init()
{
	lv_port_disp_init();
	lv_port_indev_init();
	lv_port_fs_init();
}