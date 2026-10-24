#include "stm32f4xx_hal.h"
#include "clock_config.h"
#include "kernel.h"

int main(void)
{
    HAL_Init();
    SystemClock_Config();

    if (Kernel_Init() != 0)
    {
        Error_Handler();
    }

    /* Application task creation will go here. */

    while (1)
    {
        /* Scheduler startup is not implemented yet. */
    }
}