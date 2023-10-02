/*
 * Copyright (c) 2006-2023, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-07-21     MagicLOH     do something
 */
#include <rtthread.h>

#define DBG_TAG "main"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

#include "App.h"


int main(void)
{

#ifndef PKG_USING_LVGL

	App_Init();

#endif


	return 0;
}

