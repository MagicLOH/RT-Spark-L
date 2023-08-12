#include "HAL_button.h"
#include "board.h"

#define DBG_TAG "HAL_button"
#define DBG_LVL DBG_LOG
#include "rtdbg.h"

#define EVENT_CB(ev)   if(handle->cb[ev])handle->cb[ev]((Button*)handle)

//button handle list head.
static struct Button *head_handle = NULL;

/**
  * @brief  Initializes the button struct handle.
  * @param  handle: the button handle strcut.
  * @param  pin_level: read the HAL GPIO of the connet button level.
  * @param  active_level: pressed GPIO level.
  * @param  button_id: the button id.
  * @retval None
  */
void button_init(struct Button *handle, uint8_t(*pin_level)(uint8_t), uint8_t active_level, uint8_t button_id)
{
	memset(handle, 0, sizeof(struct Button));
	handle->event = (uint8_t) NONE_PRESS;
	handle->hal_button_Level = pin_level;
	handle->button_level = handle->hal_button_Level(button_id);
	handle->active_level = active_level;
	handle->button_id = button_id;
}

/**
  * @brief  Attach the button event callback function.
  * @param  handle: the button handle strcut.
  * @param  event: trigger event type.
  * @param  cb: callback function.
  * @retval None
  */
void button_attach(struct Button *handle, PressEvent event, BtnCallback cb)
{
	handle->cb[event] = cb;
}

/**
  * @brief  Inquire the button event happen.
  * @param  handle: the button handle strcut.
  * @retval button event.
  */
PressEvent get_button_event(struct Button *handle)
{
	return (PressEvent) (handle->event);
}

/**
  * @brief  Button driver core function, driver state machine.
  * @param  handle: the button handle strcut.
  * @retval None
  */
void button_handler(struct Button *handle)
{
	// 第一次读取电平值
	uint8_t read_gpio_level = handle->hal_button_Level(handle->button_id);

	//ticks counter working..
	if ((handle->state) > 0) handle->ticks++;

	/*------------button debounce handle---------------*/
	if (read_gpio_level != handle->button_level)
	{   //not equal to prev one
		//continue read 3 times same new level change
		if (++(handle->debounce_cnt) >= DEBOUNCE_TICKS)
		{
			handle->button_level = read_gpio_level;
			handle->debounce_cnt = 0;
		}
	}
	else
	{ //leved not change ,counter reset.
		handle->debounce_cnt = 0;
	}

	/*-----------------State machine-------------------*/
	switch (handle->state)
	{
		case 0:
			if (handle->button_level == handle->active_level)
			{    //start press down
				handle->event = (uint8_t) PRESS_DOWN;
				EVENT_CB(PRESS_DOWN);
				handle->ticks = 0;
				handle->repeat = 1;
				handle->state = 1;
			}
			else
			{
				handle->event = (uint8_t) NONE_PRESS;
			}
			break;

		case 1:
			if (handle->button_level != handle->active_level)
			{   //released press up
				handle->event = (uint8_t) PRESS_UP;
				EVENT_CB(PRESS_UP);
				handle->ticks = 0;
				handle->state = 2;
			}
			else if (handle->ticks > LONG_TICKS)
			{
				handle->event = (uint8_t) LONG_PRESS_START;
				EVENT_CB(LONG_PRESS_START);
				handle->state = 5;
			}
			break;

		case 2:
			if (handle->button_level == handle->active_level)
			{ //press down again
				handle->event = (uint8_t) PRESS_DOWN;
				EVENT_CB(PRESS_DOWN);
				handle->repeat++;
				EVENT_CB(PRESS_REPEAT); // repeat hit
				handle->ticks = 0;
				handle->state = 3;
			}
			else if (handle->ticks > SHORT_TICKS)
			{ //released timeout
				if (handle->repeat == 1)
				{
					handle->event = (uint8_t) SINGLE_CLICK;
					EVENT_CB(SINGLE_CLICK);
				}
				else if (handle->repeat == 2)
				{
					handle->event = (uint8_t) DOUBLE_CLICK;
					EVENT_CB(DOUBLE_CLICK); // repeat hit
				}
				handle->state = 0;
			}
			break;

		case 3:
			if (handle->button_level != handle->active_level)
			{ //released press up
				handle->event = (uint8_t) PRESS_UP;
				EVENT_CB(PRESS_UP);
				if (handle->ticks < SHORT_TICKS)
				{
					handle->ticks = 0;
					handle->state = 2; //repeat press
				}
				else
				{
					handle->state = 0;
				}
			}
			else if (handle->ticks > SHORT_TICKS)
			{ // long press up
				handle->state = 0;
			}
			break;

		case 5:
			if (handle->button_level == handle->active_level)
			{
				//continue hold trigger
				handle->event = (uint8_t) LONG_PRESS_HOLD;
				EVENT_CB(LONG_PRESS_HOLD);

			}
			else
			{ //releasd
				handle->event = (uint8_t) PRESS_UP;
				EVENT_CB(PRESS_UP);
				handle->state = 0; //reset
			}
			break;
		default:
			handle->state = 0; //reset
			break;
	}
}

/**
  * @brief  Start the button work, add the handle into work list.
  * @param  handle: target handle strcut.
  * @retval 0: succeed. -1: already exist.
  */
int button_start(struct Button *handle)
{
	struct Button *target = head_handle;
	while (target)
	{
		if (target == handle) return -1;    //already exist.
		target = target->next;
	}
	handle->next = head_handle;
	head_handle = handle;
	return 0;
}

/**
  * @brief  Stop the button work, remove the handle off work list.
  * @param  handle: target handle strcut.
  * @retval None
  */
void button_stop(struct Button *handle)
{
	struct Button **curr;
	for (curr = &head_handle; *curr;)
	{
		struct Button *entry = *curr;
		if (entry == handle)
		{
			*curr = entry->next;
//			free(entry);
			return;//glacier add 2021-8-18
		}
		else
		{
			curr = &entry->next;
		}
	}
}

/**
  * @brief  background ticks, timer repeat invoking interval 5ms.
  * @param  None.
  * @retval None
  */
void button_ticks(void)
{
	struct Button *target;
	// 遍历所有按键 并处理相应的事件
	for (target = head_handle; target; target = target->next)
	{
		button_handler(target);
	}
}


/**==============================
 *		bsp button configure
 *===============================*/
// bsp button pinout
#define BTN_PIN_UP (GET_PIN(C, 5))
#define BTN_PIN_DOWN (GET_PIN(C, 1))
#define BTN_PIN_LEFT (GET_PIN(C, 0))
#define BTN_PIN_RIGHT (GET_PIN(C, 4))

typedef enum
{
	btnUp_id = 1,
	btnDown_id,
	btnLeft_id,
	btnRight_id
} btn_id_t;

static rt_bool_t s_btnActiveStatus = 0;

static uint8_t bsp_button_read_pin(uint8_t btn_id)
{
	switch (btn_id)
	{
		case btnUp_id:
			return rt_pin_read(BTN_PIN_UP);
			break;
		case btnDown_id:
			return rt_pin_read(BTN_PIN_DOWN);
			break;
		case btnLeft_id:
			return rt_pin_read(BTN_PIN_LEFT);
			break;
		case btnRight_id:
			return rt_pin_read(BTN_PIN_RIGHT);
			break;
		default:
			return 0;
			break;
	}
};


static void cb_timeout(void *arg)
{
	button_ticks();
//    rt_thread_mdelay(5);
}

static void Button_TimerEvent(void)
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
	if ((err = rt_timer_start(timer)) != RT_EOK)
	{
		LOG_E("rt_timer_start failed!");
		return;
	}
}

static void cb_onPressUp(void *arg)
{
	struct Button *button = (struct Button *) arg;
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

static void cb_onPressDown(void *arg)
{
	struct Button *button = (struct Button *) arg;
	//do something...
	switch (button->button_id)
	{
		case btnUp_id:
			LOG_I("btnUp press down.");
			break;
		case btnDown_id:
			LOG_I("btnDown_id press down.");
			break;
		case btnLeft_id:
			LOG_I("btnLeft_id press down.");
			break;
		case btnRight_id:
			LOG_I("btnRight_id press down.");
			break;
	}
}

static void cb_onSingleClicked(void *arg)
{
	struct Button *button = (struct Button *) arg;
	//do something...
	switch (button->button_id)
	{
		case btnUp_id:
			LOG_I("btnUp_id clicked.");
			break;
		case btnDown_id:
			LOG_I("btnDown_id clicked.");
			break;
		case btnLeft_id:
			LOG_I("btnLeft_id clicked.");
			break;
		case btnRight_id:
			LOG_I("btnRight_id clicked.");
			break;
	}
}

/**
 * @brief Button_Init
 * @param none
 */
void Button_Init(void)
{
	rt_pin_mode(BTN_PIN_UP, PIN_MODE_INPUT_PULLUP);
	rt_pin_mode(BTN_PIN_DOWN, PIN_MODE_INPUT_PULLUP);
	rt_pin_mode(BTN_PIN_LEFT, PIN_MODE_INPUT_PULLUP);
	rt_pin_mode(BTN_PIN_RIGHT, PIN_MODE_INPUT_PULLUP);

	static struct Button btnUp = {0};
	static struct Button btnDown = {0};
	static struct Button btnLeft = {0};
	static struct Button btnRight = {0};

	button_init(&btnUp, bsp_button_read_pin, s_btnActiveStatus, btnUp_id);
	button_init(&btnDown, bsp_button_read_pin, s_btnActiveStatus, btnDown_id);
	button_init(&btnLeft, bsp_button_read_pin, s_btnActiveStatus, btnLeft_id);
	button_init(&btnRight, bsp_button_read_pin, s_btnActiveStatus, btnRight_id);

	button_attach(&btnUp, SINGLE_CLICK, &cb_onSingleClicked);
	button_attach(&btnUp, PRESS_UP, &cb_onPressUp);
	button_attach(&btnUp, PRESS_DOWN, &cb_onPressDown);

	button_attach(&btnDown, SINGLE_CLICK, &cb_onSingleClicked);
	button_attach(&btnDown, PRESS_UP, &cb_onPressUp);
	button_attach(&btnDown, PRESS_DOWN, &cb_onPressDown);

	button_attach(&btnRight, SINGLE_CLICK, &cb_onSingleClicked);
	button_attach(&btnRight, PRESS_UP, &cb_onPressUp);
	button_attach(&btnRight, PRESS_DOWN, &cb_onPressDown);

	button_attach(&btnLeft, SINGLE_CLICK, &cb_onSingleClicked);
	button_attach(&btnLeft, PRESS_UP, &cb_onPressUp);
	button_attach(&btnLeft, PRESS_DOWN, &cb_onPressDown);

	button_start(&btnUp);
	button_start(&btnDown);
	button_start(&btnRight);
	button_start(&btnLeft);

	Button_TimerEvent(); // 注册定时器监控 5ms间隔
}