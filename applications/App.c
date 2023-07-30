//
// Created by ds on 2023/7/27.
//

#include "App.h"

#include <rtthread.h>

#define DBG_TAG "App"
#define DBG_LVL DBG_LOG

#include <rtdbg.h>

#include "HAL_button.h"

#if 0
#include "lvgl.h"
#include "lv_event.h"

static void event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);

    if (code == LV_EVENT_CLICKED)
    {
        LV_LOG_USER("Clicked");
    }
    else if (code == LV_EVENT_VALUE_CHANGED)
    {
        LV_LOG_USER("Toggled");
    }
}
#endif

static void cb_timeout(void *arg)
{
    button_ticks();
//    rt_thread_mdelay(5);
}

static void App_Button(void)
{
    rt_err_t err = RT_EOK;
    static rt_timer_t timer;

    // 5ms period timer
    timer = rt_timer_create("periodTimer", cb_timeout, RT_NULL,
                            TICKS_INTERVAL, RT_TIMER_FLAG_PERIODIC);
    RT_ASSERT(timer);
    if (err = rt_timer_start(timer) != RT_EOK)
    {
        LOG_E("rt_timer_start failed!");
        return;
    }
}

int App_Init(void)
{
    App_Button();
#if 0
    lv_obj_t *label;

    lv_obj_t *btn1 = lv_btn_create(lv_scr_act());
    lv_obj_add_event_cb(btn1, event_handler, LV_EVENT_ALL, NULL);
    lv_obj_align(btn1, LV_ALIGN_CENTER, 0, -40);

    label = lv_label_create(btn1);
    lv_label_set_text(label, "Button");
    lv_obj_center(label);

    lv_obj_t *btn2 = lv_btn_create(lv_scr_act());
    lv_obj_add_event_cb(btn2, event_handler, LV_EVENT_ALL, NULL);
    lv_obj_align(btn2, LV_ALIGN_CENTER, 0, 40);
    lv_obj_add_flag(btn2, LV_OBJ_FLAG_CHECKABLE);
    lv_obj_set_height(btn2, LV_SIZE_CONTENT);

    label = lv_label_create(btn2);
    lv_label_set_text(label, "Toggle");
    lv_obj_center(label);
#endif
    LOG_I("App_Init done.");
}
INIT_APP_EXPORT(App_Init);
