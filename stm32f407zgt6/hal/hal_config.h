#ifndef __HAL_CONFIG_H
#define __HAL_CONFIG_H

/* button */
#define BSP_PIN_BTN_UP      (GET_PIN(C, 5))
#define BSP_PIN_BTN_DOWN    (GET_PIN(C, 1))
#define BSP_PIN_BTN_LEFT    (GET_PIN(C, 0))
#define BSP_PIN_BTN_RIGHT   (GET_PIN(C, 4))

#define BUTTON_THREAD_NAME "button"
#define BUTTON_THREAD_PRIORITY 20
#define BUTTON_THREAD_STACK_SIZE 1024


#endif
