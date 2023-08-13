//
// Created by ds on 2023/7/27.
//

#ifndef ___HAL_H_
#define ___HAL_H_

#include "HAL_button.h"
#include "HAL_lcd.h"
#include "HAL_rtc.h"
#include "HAL_sdcard.h"

#ifdef __cplusplus
extern "C" {
#endif

//int myHAL_Init(void);
void HAL_Update(void);

#ifdef __cplusplus
}
#endif


#endif //_HAL_H_
