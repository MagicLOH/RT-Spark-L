#include <lvgl.h>

extern void lv_user_demo_init(void);

extern void lv_user_example_init(void);

extern void lv_example_btn_1(void);

void lv_user_gui_init(void)
{
//    lv_user_demo_init();
    LV_FONT_DECLARE(lv_font_chn_xingkai);
    lv_obj_t *label;

    lv_obj_t *btn1 = lv_btn_create(lv_scr_act());
    lv_obj_align(btn1, LV_ALIGN_CENTER, 0, -40);

    label = lv_label_create(btn1);
    lv_label_set_text(label, "按钮");
    lv_obj_set_style_text_font(label, &lv_font_chn_xingkai, 0);
    lv_obj_center(label);

    lv_obj_t *btn2 = lv_btn_create(lv_scr_act());
    lv_obj_align(btn2, LV_ALIGN_CENTER, 0, 40);
    lv_obj_add_flag(btn2, LV_OBJ_FLAG_CHECKABLE);
    lv_obj_set_height(btn2, LV_SIZE_CONTENT);

    label = lv_label_create(btn2);
    lv_label_set_text(label, "翻转");
    lv_obj_set_style_text_font(label, &lv_font_chn_xingkai, 0);
    lv_obj_center(label);
}
