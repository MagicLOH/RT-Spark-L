//
// Created by ds on 2023/7/27.
//

#include "HAL_rtc.h"
#include <rtthread.h>
#include <rtdevice.h>

#define RTC_NAME "rtc"

#define DBG_TAG "HAL_rtc"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

#include "Config.h"

void RTC_Update(void)
{
	
}

void RTC_Init(void)
{
	rt_device_t rtc_dev = NULL;
	rt_err_t err = RT_EOK;

	/* 寻找设备 */
	rtc_dev = rt_device_find(RTC_NAME);
	RT_ASSERT(rtc_dev != NULL);
	LOG_I("rt_device_find(%s) done.", RTC_NAME);

	/* 初始化RTC设备 */
	err = rt_device_open(rtc_dev, 0);
	if (err != RT_EOK)
	{
		LOG_I("rt_device_open(%s, 0) done.", RTC_NAME);
		return;
	}

//	/* 获取时间 */
//	time_t now = time(RT_NULL);
//	rt_kprintf("%s", ctime(&now));

	LOG_I("RTC_Init done.");
}