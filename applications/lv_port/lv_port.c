#include <lvgl.h>

extern void lv_user_demo_init(void);

extern void lv_user_example_init(void);

extern void lv_example_btn_1(void);

#define DBG_TAG "lv_port"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

LV_FONT_DECLARE(lv_font_chn_xingkai); // 使用华文行楷字体

void lv_user_gui_init(void)
{
//    lv_user_demo_init();
//    lv_fs_file_t lv_file;
//    lv_fs_res_t lv_res = lv_fs_open(&lv_file, "S:/sdcard/font/lv_font_chn_xingkai.c", LV_FS_MODE_RD);
//    if (LV_FS_RES_OK != lv_res)
//    {
//        LOG_E("lv_fs_open failed! lv_res(%d)", lv_res);
//        return;
//    }
//    LOG_I("lv_fs_open done.");

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
