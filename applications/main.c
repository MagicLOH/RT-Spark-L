/**
 * <rt-spark> official develop board
 * Author: loh
 * Date: create on 2023-07-28
 */

#include <rtthread.h>

#define DBG_TAG "main"
#define DBG_LVL         DBG_LOG

#include <rtdbg.h>

#include "App.h"

int main(void)
{
    App_Init();

    return 0;
}
