#include "Reader.h"

using namespace Page;
#if 0
Reader::Reader()
//    : Model()
{

}

Reader::~Reader()
{

}

void Reader::onCustomAttrConfig()
{
    LV_LOG_USER(__func__);
    SetCustomCacheEnable(true);
    SetCustomLoadAnimType(PageManager::LOAD_ANIM_OVER_BOTTOM, 1000, lv_anim_path_bounce);
}

void Reader::onViewLoad()
{
    LV_LOG_USER(__func__);
    View.Create(_root);
    lv_label_set_text(View.ui.labelTitle, _Name);

    AttachEvent(_root);

//    Model.TickSave = Model.GetData();
}

void Reader::onViewDidLoad()
{
    LV_LOG_USER(__func__);
}

void Reader::onViewWillAppear()
{
    LV_LOG_USER(__func__);
    Param_t param;
    param.color = lv_color_white();
    param.time = 1000;

    PAGE_STASH_POP(param);

    lv_obj_set_style_bg_color(_root, param.color, LV_PART_MAIN);
    lv_obj_set_style_bg_opa(_root, LV_OPA_COVER, LV_PART_MAIN);
    timer = lv_timer_create(onTimerUpdate, param.time, this);
}

void Reader::onViewDidAppear()
{
    LV_LOG_USER(__func__);
}

void Reader::onViewWillDisappear()
{
    LV_LOG_USER(__func__);
}

void Reader::onViewDidDisappear()
{
    LV_LOG_USER(__func__);
    lv_timer_del(timer);
}

void Reader::onViewUnload()
{
    LV_LOG_USER(__func__);
}

void Reader::onViewDidUnload()
{
    LV_LOG_USER(__func__);
}

void Reader::AttachEvent(lv_obj_t *obj)
{
    lv_obj_set_user_data(obj, this);
    lv_obj_add_event_cb(obj, onEvent, LV_EVENT_ALL, this);
}

void Reader::Update()
{
    lv_label_set_text_fmt(View.ui.labelTick, "tick = %d save = %d", Model.GetData(), Model.TickSave);
}

void Reader::onTimerUpdate(lv_timer_t *timer)
{
    Template *instance = (Template *)timer->user_data;

    instance->Update();
}

void Reader::onEvent(lv_event_t *event)
{
    Novel *instance = (Novel *)lv_event_get_user_data(event);
    LV_ASSERT_NULL(instance);

    lv_obj_t *obj = lv_event_get_current_target(event);
    lv_event_code_t code = lv_event_get_code(event);

    if (obj == instance->_root)
    {
        if (code == LV_EVENT_SHORT_CLICKED || code == LV_EVENT_LEAVE)
        {
            instance->_Manager->Pop();
        }
    }
}
#endif