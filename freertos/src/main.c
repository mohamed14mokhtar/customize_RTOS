#include "stm32f4xx_hal.h"
#include "clock_config.h"
int main(void)
{
    HAL_Init();
    SystemClock_Config();

    while (1)
    {
        /* FreeRTOS will be added after this bare-metal project builds. */
    }
}

