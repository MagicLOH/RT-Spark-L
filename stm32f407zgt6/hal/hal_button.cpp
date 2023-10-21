#include "hal.h"

#include "rtthread.h"
#define DBG_TAG         "hal.button"
#define DBG_LVL DBG_LOG
#include "rtdbg.h"

#define ARRAY_SIZE(x) ((sizeof(x)) / (sizeof((x)[0])))

#ifdef OFFLINE_PKG_USING_MULTIBUTTON
#include "multi_button.h"
#include "board.h"

static rt_bool_t s_btnActiveStatus = 0;

#ifdef OFFLINE_PKG_USING_LVGL
static rt_uint8_t s_act_key = 0; // actived key value

rt_uint8_t hal::get_act_key_value()
{
	return s_act_key;
}
#endif

static uint8_t bsp_btn_read_pin(uint8_t btn_id)
{
	switch (btn_id)
	{
		case BTN_UP_ID:
			return rt_pin_read(BSP_PIN_BTN_UP);
		case BTN_DOWN_ID:
			return rt_pin_read(BSP_PIN_BTN_DOWN);
		case BTN_LEFT_ID:
			return rt_pin_read(BSP_PIN_BTN_LEFT);
		case BTN_RIGHT_ID:
			return rt_pin_read(BSP_PIN_BTN_RIGHT);
		default:
			return 0;
	}
}

__WEAK void hal::btn_on_pressed(void *arg)
{
#ifdef OFFLINE_PKG_USING_LVGL
	struct Button *b = (struct Button *)arg;
	s_act_key = b->button_id; // get current active key value
#endif

	RT_UNUSED(arg);
	// do something ...
	LOG_W("Event press, do something...");
}

__WEAK void hal::btn_on_released(void *arg)
{
#ifdef OFFLINE_PKG_USING_LVGL
	s_act_key = 0; // clear current active key value
#endif

	RT_UNUSED(arg);
	// do something ...
	LOG_W("Event release, do something...");
}

__WEAK void hal::btn_on_long_pressing(void *btn)
{
	RT_UNUSED(btn);
	// do something ...
	LOG_W("Event LongPressing, do something...");
}

void btn_thread_entry(void *args)
{
	LOG_I("button thread create done.");
	while (1)
	{
		button_ticks();
		rt_thread_mdelay(5);
	}
}

/**
 * @brief button_init
 * @param none
 */
void hal::btn_init(void)
{
	static struct Button st_btnUp = {0};
	static struct Button st_btnDown = {0};
	static struct Button st_btnLeft = {0};
	static struct Button st_btnRight = {0};

	static btn_context_t btn_ctx[] = {
		{&st_btnUp,    BSP_PIN_BTN_UP,    BTN_UP_ID},
		{&st_btnDown,  BSP_PIN_BTN_DOWN,  BTN_DOWN_ID},
		{&st_btnLeft,  BSP_PIN_BTN_LEFT,  BTN_LEFT_ID},
		{&st_btnRight, BSP_PIN_BTN_RIGHT, BTN_RIGHT_ID},
	};

	for (rt_uint8_t i = 0; i < ARRAY_SIZE(btn_ctx); ++i)
	{
		/* Config gpio */
		rt_pin_mode(btn_ctx[i].btn_pin, PIN_MODE_INPUT_PULLUP);

		/* Register the button */
		button_init(btn_ctx[i].btn_handler, bsp_btn_read_pin,
		            s_btnActiveStatus, btn_ctx[i].btn_num);
		button_attach(btn_ctx[i].btn_handler, PRESS_UP, &btn_on_released);
		button_attach(btn_ctx[i].btn_handler, PRESS_DOWN, &btn_on_pressed);
		button_attach(btn_ctx[i].btn_handler, LONG_PRESS_HOLD, &btn_on_long_pressing);

		/* Start enter button event */
		button_start(btn_ctx[i].btn_handler);
	}

	rt_thread_t tid = rt_thread_create(BUTTON_THREAD_NAME, btn_thread_entry, (void *)NULL,
	                                   BUTTON_THREAD_STACK_SIZE, BUTTON_THREAD_PRIORITY, 10);
	RT_ASSERT(tid != RT_NULL);
	rt_thread_startup(tid);
}
#endif

