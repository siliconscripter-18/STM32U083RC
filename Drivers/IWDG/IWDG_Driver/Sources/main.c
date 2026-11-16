/******************************************************************************
 * @file    main.c
 * @brief   IWDG watchdog test application.
 *
 * @author  Balaji M
 * @target  STM32U083RC
 ******************************************************************************/

#include "stm32u083xx.h"
#include <stdint.h>
#include "gpio.h"
#include "tim1.h"
#include "IWDG.h"
#include "uart.h"

/******************************************************************************
 * @brief Main application entry point.
 *
 * Initializes GPIO, TIM1, UART and IWDG.
 * Detects watchdog resets and reports the reset cause through UART.
 * Pressing B1 intentionally stops watchdog refresh to trigger a reset.
 ******************************************************************************/
int main(void)
{
    GPIO_Init();
    TIM1_Init();
    UART_Init();

    /* Check whether the previous reset was caused by IWDG */
    if (RCC->CSR & RCC_CSR_IWDGRSTF)
    {
        UART_SendString("IWDG_RESET\r\n");
    }

    /* Clear reset flags */
    RCC->CSR |= RCC_CSR_RMVF;

    IWDG_Init();

    while (1)
    {
        /* Toggle LED during normal execution */
        Toggle_LED();

        /* B1 pressed = PC13 LOW */
        if (!(GPIOC->IDR & GPIO_IDR_ID13))
        {
            /* Turn ON LED before entering the intentional hang */
            GPIOA->ODR |= GPIO_ODR_OD5;

            while (1)
            {
                /*
                 * Stay here without refreshing IWDG.
                 * Watchdog timeout will reset the MCU.
                 * LED remains ON until the reset occurs.
                 */
            }
        }

        /* Refresh watchdog during normal operation */
        IWDG_Refresh();

        delay_ms(200);
    }
}