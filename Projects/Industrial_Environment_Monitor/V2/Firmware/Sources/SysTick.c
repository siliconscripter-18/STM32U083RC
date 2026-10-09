/******************************************************************************
 * @file    SysTick.c
 * @brief   SysTick timer and millisecond time-base interface.
 *
 *          Provides a 1 ms system time base and a blocking millisecond delay
 *          function used by the application.
 *
 * @author  Balaji M
 * @target  STM32U083RC
 ******************************************************************************/

#include "stm32u083xx.h"
#include "SysTick.h"



/******************************************************************************
 * @brief Initializes SysTick for a 1 ms time base.
 ******************************************************************************/
void SysTick_Init(void)
{
    SysTick->LOAD = 3999U;
    SysTick->VAL = 0U;
    SysTick->CTRL |= ((1U << 0) | (1U << 1) | (1U << 2));
}


volatile uint32_t SysTick_ms = 0;


/******************************************************************************
 * @brief SysTick interrupt handler.
 *
 *        Increments the millisecond system counter on every SysTick
 *        interrupt.
 ******************************************************************************/
void SysTick_Handler(void)
{
    SysTick_ms++;
}


/******************************************************************************
 * @brief Provides a blocking millisecond delay.
 *
 * @param ms Delay duration in milliseconds.
 ******************************************************************************/
void Delay_ms(uint32_t ms)
{
    uint32_t start = SysTick_ms;

    while ((SysTick_ms - start) < ms)
    {
    }
}
