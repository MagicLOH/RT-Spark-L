//
// Created by ds on 2023/7/27.
//

#ifndef _HAL_BUTTON_H_
#define _HAL_BUTTON_H_

#include <rtdef.h>


// bsp button pino
#define BSP_PIN_BTN_UP      (GET_PIN(C, 5))
#define BSP_PIN_BTN_DOWN    (GET_PIN(C, 1))
#define BSP_PIN_BTN_LEFT    (GET_PIN(C, 0))
#define BSP_PIN_BTN_RIGHT   (GET_PIN(C, 4))
typedef enum
{
	btnUp_id = 0,
	btnDown_id,
	btnLeft_id,
	btnRight_id
} btn_id_t;

#ifdef __cplusplus
extern "C"
{
#endif

void Button_Init(void);
void Button_Update(void);

#ifdef __cplusplus
}
#endif

#endif //_HAL_BUTTON_H_
