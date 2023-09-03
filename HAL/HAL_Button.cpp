#include "HAL.h"

#include "HAL_Config.h"
#include "MultiButton.h"
#include "board.h"

#define DBG_TAG "HAL.button"
#define DBG_LVL DBG_LOG
#include "rtdbg.h"

#include "ReaderModel.h"


static rt_bool_t s_btnActiveStatus = 0;
static uint8_t s_btn_id = 0;

static uint8_t bsp_button_read_pin(uint8_t btn_id)
{
    switch (btn_id)
    {
        case btnUp_id:
            return rt_pin_read(BSP_PIN_BTN_UP);
        case btnDown_id:
            return rt_pin_read(BSP_PIN_BTN_DOWN);
        case btnLeft_id:
            return rt_pin_read(BSP_PIN_BTN_LEFT);
        case btnRight_id:
            return rt_pin_read(BSP_PIN_BTN_RIGHT);
        default:
            return 0;
    }
};
#if 0
static void cb_timeout(void *arg)
{
//	button_ticks();
//    rt_thread_mdelay(5);
}
static void Button_TimerReigister(void)
{
    rt_err_t err = RT_EOK;
    static rt_timer_t timer;

    // 5ms period timer
    timer = rt_timer_create(
        "periodTimer",
        cb_timeout,
        RT_NULL,
        TICKS_INTERVAL,
        RT_TIMER_FLAG_PERIODIC
    );
    RT_ASSERT(timer);
    err = rt_timer_start(timer);
    if (err != RT_EOK)
    {
        LOG_E("periodTimer for btn start failed!");
        return;
    }
}
#endif

uint8_t HAL::Button_GetID()
{
//    LOG_I("BTN GET ID: %d", s_btn_id);
    return s_btn_id;
}

using namespace Page;

ReaderModel reader;

void HAL::Button_onPressed(void *arg)
{
    struct Button *button = (struct Button *)arg;
    s_btn_id = button->button_id;
    //do something...
    switch (button->button_id)
    {
        case btnUp_id:
        {
            LOG_I("btnUp press down.");
            // send key value to queue
            reader.toPrevPage();
            break;
        }
        case btnDown_id:
        {
            LOG_I("btnDown_id press down.");
            reader.toNextPage();
            break;
        }
        case btnLeft_id:
        {
            LOG_I("btnLeft_id press down.");
            break;
        }
        case btnRight_id:
        {
            LOG_I("btnRight_id press down.");
            break;
        }
    }
}

void HAL::Button_onReleased(void *arg)
{
    struct Button *button = (struct Button *)arg;
    s_btn_id = 0;
    //do something...
    switch (button->button_id)
    {
        case btnUp_id:
            LOG_I("btnUP pressed up.");
            break;
        case btnDown_id:
            LOG_I("btnDown pressed up.");
            break;
        case btnLeft_id:
            LOG_I("btnLeft pressed up.");
            break;
        case btnRight_id:
            LOG_I("btnRight pressed up.");
            break;
    }

}

void HAL::Button_onLongPressing(void *btn)
{
    struct Button *button = (struct Button *)btn;
//    Button_GetID(&(button->button_id));
    //do something...
    switch (button->button_id)
    {
        case btnUp_id:
        {
            LOG_I("btnUp_id long press.");
            break;
        }
        case btnDown_id:
        {
            LOG_I("btnDown_id long press.");
            break;
        }
        case btnLeft_id:
        {
            LOG_I("btnLeft_id long press.");
            break;
        }
        case btnRight_id:
        {
            LOG_I("btnRight_id long press.");
            break;
        }
    }
}

void HAL::Button_Update(void)
{
    button_ticks(); // 执行状态机
}

/**
 * @brief Button_Init
 * @param none
 */
void HAL::Button_Init(void)
{
    rt_pin_mode(BSP_PIN_BTN_UP, PIN_MODE_INPUT_PULLUP);
    rt_pin_mode(BSP_PIN_BTN_DOWN, PIN_MODE_INPUT_PULLUP);
    rt_pin_mode(BSP_PIN_BTN_LEFT, PIN_MODE_INPUT_PULLUP);
    rt_pin_mode(BSP_PIN_BTN_RIGHT, PIN_MODE_INPUT_PULLUP);

    static struct Button st_btnUp = {0};
    static struct Button st_btnDown = {0};
    static struct Button st_btnLeft = {0};
    static struct Button st_btnRight = {0};

    button_init(&st_btnUp, bsp_button_read_pin, s_btnActiveStatus, btnUp_id);
    button_init(&st_btnDown, bsp_button_read_pin, s_btnActiveStatus, btnDown_id);
    button_init(&st_btnLeft, bsp_button_read_pin, s_btnActiveStatus, btnLeft_id);
    button_init(&st_btnRight, bsp_button_read_pin, s_btnActiveStatus, btnRight_id);

    button_attach(&st_btnUp, LONG_PRESS_HOLD, &Button_onLongPressing);
    button_attach(&st_btnUp, PRESS_UP, &Button_onReleased);
    button_attach(&st_btnUp, PRESS_DOWN, &Button_onPressed);

    button_attach(&st_btnDown, LONG_PRESS_HOLD, &Button_onLongPressing);
    button_attach(&st_btnDown, PRESS_UP, &Button_onReleased);
    button_attach(&st_btnDown, PRESS_DOWN, &Button_onPressed);

    button_attach(&st_btnRight, LONG_PRESS_HOLD, &Button_onLongPressing);
    button_attach(&st_btnRight, PRESS_UP, &Button_onReleased);
    button_attach(&st_btnRight, PRESS_DOWN, &Button_onPressed);

    button_attach(&st_btnLeft, LONG_PRESS_HOLD, &Button_onLongPressing);
    button_attach(&st_btnLeft, PRESS_UP, &Button_onReleased);
    button_attach(&st_btnLeft, PRESS_DOWN, &Button_onPressed);

    button_start(&st_btnUp);
    button_start(&st_btnDown);
    button_start(&st_btnRight);
    button_start(&st_btnLeft);
    
    reader.loadNovel("/sdcard/novels/bullshit.txt", "bullshit");
}
