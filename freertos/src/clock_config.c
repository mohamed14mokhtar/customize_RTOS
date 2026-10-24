#include "stm32f4xx_hal.h"
#include "clock_config.h"

void SystemClock_Config(void)
{
    RCC_OscInitTypeDef osc_config = {0};
    RCC_ClkInitTypeDef clk_config = {0};

    /* Enable the power controller and select the voltage scale
       required for 84 MHz operation. */
    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

    /* Use the 25 MHz external crystal as the PLL source:
       25 MHz / 25 * 336 / 4 = 84 MHz. */
    osc_config.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    osc_config.HSEState = RCC_HSE_ON;
    osc_config.PLL.PLLState = RCC_PLL_ON;
    osc_config.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    osc_config.PLL.PLLM = 25;
    osc_config.PLL.PLLN = 336;
    osc_config.PLL.PLLP = RCC_PLLP_DIV4;
    osc_config.PLL.PLLQ = 7;

    if (HAL_RCC_OscConfig(&osc_config) != HAL_OK)
    {
        Error_Handler();
    }

    /* Select PLL as SYSCLK. Keep APB1 at or below 42 MHz and APB2
       at or below 84 MHz. */
    clk_config.ClockType = RCC_CLOCKTYPE_SYSCLK
                         | RCC_CLOCKTYPE_HCLK
                         | RCC_CLOCKTYPE_PCLK1
                         | RCC_CLOCKTYPE_PCLK2;
    clk_config.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    clk_config.AHBCLKDivider = RCC_SYSCLK_DIV1;
    clk_config.APB1CLKDivider = RCC_HCLK_DIV2;
    clk_config.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&clk_config, FLASH_LATENCY_2) != HAL_OK)
    {
        Error_Handler();
    }
}

void Error_Handler(void)
{
    __disable_irq();

    while (1)
    {
    }
}