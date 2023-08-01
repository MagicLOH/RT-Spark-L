/** \ | /
 * - RT -     Thread Operating System
 * / | \     4.1.1 build Jul 30 2023 20:00:00
 * 2006 - 2023 Copyright by RT-Thread team
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
