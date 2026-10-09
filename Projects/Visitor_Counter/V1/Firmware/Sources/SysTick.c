/******************************************************************************
 * @file    SysTick.c
 * @brief   SysTick driver for 1 ms system tick and blocking delay.
 *
 * @author  Balaji M
 * @target  STM32U083RC
 ******************************************************************************/

#include "stm32u083xx.h"
#include "SysTick.h"

volatile uint32_t SysTick_ms = 0;

/******************************************************************************
 * @brief Initializes the SysTick timer to generate a 1 ms interrupt.
 ******************************************************************************/
void SysTick_Init(void)
{
    /* Reload value for 1 ms interrupt (4 MHz system clock) */
    SysTick->LOAD = 3999U;

    /* Clear the current counter value */
    SysTick->VAL = 0U;

    /* Enable SysTick timer, SysTick interrupt, and processor clock */
    SysTick->CTRL |= (1U << 0) | (1U << 1) | (1U << 2);
}

/******************************************************************************
 * @brief SysTick interrupt handler.
 *
 * @note Increments the millisecond system tick counter.
 ******************************************************************************/
void SysTick_Handler(void)
{
    SysTick_ms++;
}

/******************************************************************************
 * @brief Generates a blocking delay.
 *
 * @param ms Delay duration in milliseconds.
 ******************************************************************************/
void Delay_ms(uint32_t ms)
{
    uint32_t start = SysTick_ms;

    while ((SysTick_ms - start) < ms);
}
