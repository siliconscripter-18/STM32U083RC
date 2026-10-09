/******************************************************************************
 * @file    SysTick.c
 * @brief   SysTick driver providing a 1 ms system tick and blocking delays.
 *
 *          Maintains a millisecond system counter used by application modules
 *          for timing, timeouts, debounce intervals, and warning durations.
 *
 * @author  Balaji M
 * @target  STM32U083RC
 ******************************************************************************/

#include "stm32u083xx.h"
#include "SysTick.h"


/* Millisecond system tick counter */
volatile uint32_t SysTick_ms = 0;


/******************************************************************************
 * @brief Initializes SysTick to generate a 1 ms periodic interrupt.
 *
 *        The reload value is configured for a 4 MHz system clock.
 ******************************************************************************/
void SysTick_Init(void)
{
    /* Configure reload value for a 1 ms interrupt at 4 MHz */
    SysTick->LOAD = 3999U;

    /* Clear the current counter value */
    SysTick->VAL = 0U;

    /* Enable SysTick, its interrupt, and use the processor clock */
    SysTick->CTRL |= (1U << 0) |
                     (1U << 1) |
                     (1U << 2);
}


/******************************************************************************
 * @brief Handles the SysTick interrupt.
 *
 *        Increments the global millisecond counter used throughout the
 *        application for non-hardware timing operations.
 ******************************************************************************/
void SysTick_Handler(void)
{
    SysTick_ms++;
}


/******************************************************************************
 * @brief Generates a blocking delay.
 *
 * @param ms Delay duration in milliseconds.
 *
 * @note The CPU remains in the delay loop until the requested duration
 *       has elapsed. The SysTick interrupt continues to update SysTick_ms.
 ******************************************************************************/
void Delay_ms(uint32_t ms)
{
    uint32_t start = SysTick_ms;

    while ((SysTick_ms - start) < ms)
    {
        /* Wait for the requested interval to elapse */
    }
}
