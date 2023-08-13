#include "HAL.h"

#define DBG_TAG "HAL"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

void HAL_Update(void)
{
//	RTC_Update();
	Button_Update();

}

/**====================
 * 		Hal init
 *=====================*/
int myHAL_Init(void)
{
	RTC_Init();

	Button_Init();

	SD_Init(); // manually mount sd

	

	LOG_I("myHAL_Init done.");
}
INIT_APP_EXPORT(myHAL_Init);