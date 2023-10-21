#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>

#define GPIO_LED_B    GET_PIN(F, 11)

//#define DBG_TAG         "main"
//#define DBG_LVL DBG_LOG
//#include "rtdbg.h"

int main(void)
{
	int count =  1;
	rt_pin_mode(GPIO_LED_B, PIN_MODE_OUTPUT);

	while (count++)
	{
		rt_pin_write(GPIO_LED_B, PIN_HIGH);
		rt_thread_mdelay(500);
		rt_pin_write(GPIO_LED_B, PIN_LOW);
		rt_thread_mdelay(500);
	}
	
	return 0;
}
