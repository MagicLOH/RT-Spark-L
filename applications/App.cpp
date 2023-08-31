#include "App.h"

#include <rtthread.h>
#define DBG_TAG "App"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

#ifdef PKG_USING_LVGL
#include <lvgl.h>
#include "DataProc.h"
#include "PageManager.h"
#include "AppFactory.h"
#include "ResourcePool.h"
#include "StatusBar.h"

#define ACCOUNT_SEND_CMD(ACT, CMD) \
do{ \
    DataProc::ACT##_Info_t info; \
    DATA_PROC_INIT_STRUCT(info); \
    info.cmd = DataProc::CMD; \
    DataProc::Center()->AccountMain.Notify(#ACT, &info, sizeof(info)); \
}while(0)

#endif /* PKG_USING_LVGL */
/**
 * @brief Applications Init Entry
 */
void App_Init()
{
#ifdef PKG_USING_LVGL
	static AppFactory factory;
	static PageManager manager(&factory);

	/* Make sure the default group exists */
	if (!lv_group_get_default())
	{
		lv_group_t *group = lv_group_create();
		lv_group_set_default(group);
	}

	/* Initialize the data processing node */
	DataProc_Init();
	ACCOUNT_SEND_CMD(SysConfig, SYSCONFIG_CMD_LOAD);

	/* Set screen style */
	lv_obj_t *scr = lv_scr_act();
	lv_obj_remove_style_all(scr);
	lv_obj_set_style_bg_opa(lv_scr_act(), LV_OPA_COVER, 0);
	lv_obj_set_style_bg_color(lv_scr_act(), lv_color_black(), 0);
	lv_obj_clear_flag(scr, LV_OBJ_FLAG_SCROLLABLE);
	lv_disp_set_bg_color(lv_disp_get_default(), lv_color_black());

	/* Set root default style */
	static lv_style_t rootStyle;
	lv_style_init(&rootStyle);
	lv_style_set_width(&rootStyle, LV_HOR_RES);
	lv_style_set_height(&rootStyle, LV_VER_RES);
	lv_style_set_bg_opa(&rootStyle, LV_OPA_COVER);
	lv_style_set_bg_color(&rootStyle, lv_color_black());
	lv_style_set_border_color(&rootStyle, lv_color_black());
	manager.SetRootDefaultStyle(&rootStyle);

	/* Initialize resource pool */
	ResourcePool::Init();

	/* Initialize status bar */
	Page::StatusBar_Create(lv_layer_top());

	/* Initialize pages */
	manager.Install("Template", "Pages/_Template");
	manager.Install("SystemInfos", "Pages/SystemInfos");
	manager.Install("Startup", "Pages/Startup");

	manager.SetGlobalLoadAnimType(PageManager::LOAD_ANIM_OVER_TOP);

	manager.Push("Pages/Startup"); // default display page
#endif

	LOG_I("App_Init done.");
}


