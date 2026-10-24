
/******************************************************************************
 * @file    SysTick.c
 * @brief   SysTick driver providing a 1 ms system tick.
 *
 *          Maintains a millisecond system counter that can be used by
 *          application modules for timing intervals, timeouts, debounce
 *          operations, and periodic tasks.
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
 *        Configures the reload value for a 4 MHz processor clock and
 *        resets the millisecond counter before enabling SysTick.
 ******************************************************************************/
void SysTick_Init(void)
{
    /* Reset the millisecond system tick counter */
    SysTick_ms = 0;

    /* Configure reload value: (4 MHz / 1000 Hz) - 1 = 3999 */
    SysTick->LOAD = 3999U;

    /* Clear the current SysTick counter value */
    SysTick->VAL = 0U;

    /* Enable SysTick interrupt, processor clock, and counter */
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk |SysTick_CTRL_ENABLE_Msk;
}


/******************************************************************************
 * @brief Handles the SysTick interrupt.
 *
 *        Increments the global millisecond counter every 1 ms.
 *        The counter is used by application modules for time tracking.
 ******************************************************************************/
void SysTick_Handler(void)
{
    /* Increment the millisecond system tick counter */
    SysTick_ms++;
}
