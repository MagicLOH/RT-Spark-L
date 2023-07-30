//
// Created by ds on 2023/7/27.
//

#include "HAL_rtc.h"
#include "rtthread.h"
#include "rtdevice.h"

#define RTC_NAME "rtc"
#if 0
rt_err_t RTC_Init(void)
{
	rt_err_t err = RT_EOK;
	time_t now;
	rt_device_t device = RT_NULL;

	/* 寻找设备 */
	device = rt_device_find(RTC_NAME);
	if (!device)
	{
		rt_kprintf("find %s failed!", RTC_NAME);
		return RT_ERROR;
	}

	/* 初始化RTC设备 */
	if (rt_device_open(device, 0) != RT_EOK)
	{
		rt_kprintf("open %s failed!", RTC_NAME);
		return RT_ERROR;
	}

	/* 设置日期 */
	err = set_date(2023, 7, 1);
	if (err != RT_EOK)
	{
		rt_kprintf("set RTC date failed\n");
		return err;
	}

	/* 设置时间 */
	err = set_time(11, 30, 50);
	if (err != RT_EOK)
	{
		rt_kprintf("set RTC time failed\n");
		return err;
	}

	/* 获取时间 */
	now = time(RT_NULL);
	rt_kprintf("%s\n", ctime(&now));

	/* 延时1秒 */
	rt_thread_mdelay(1000);

	/* 获取时间 */
	now = time(RT_NULL);
	rt_kprintf("%s\n", ctime(&now));

	return err;
}
#endif