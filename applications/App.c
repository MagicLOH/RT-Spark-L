//
// Created by ds on 2023/7/27.
//

#include "App.h"
#include "HAL.h"

#include <rtthread.h>

#define DBG_TAG "App"
#define DBG_LVL DBG_LOG

#include <rtdbg.h>


int App_Init(void)
{
	rt_err_t err = RT_EOK;
	

	LOG_I("App_Init done.");
	return err;
}
