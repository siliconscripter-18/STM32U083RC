/******************************************************************************
 * @file    SysTick.h
 * @brief   SysTick timer and millisecond time-base interface.
 *
 *          Provides the system millisecond counter, SysTick initialization,
 *          interrupt handler, and millisecond delay function.
 *
 * @author  Balaji M
 * @target  STM32U083RC
 ******************************************************************************/

#ifndef SOURCES_SYSTICK_H_
#define SOURCES_SYSTICK_H_

#include <stdint.h>

extern volatile uint32_t SysTick_ms;


/******************************************************************************
 * @brief Initializes SysTick for a 1 ms time base.
 ******************************************************************************/
void SysTick_Init(void);


/******************************************************************************
 * @brief SysTick interrupt handler.
 ******************************************************************************/
void SysTick_Handler(void);


/******************************************************************************
 * @brief Provides a blocking millisecond delay.
 *
 * @param ms Delay duration in milliseconds.
 ******************************************************************************/
void Delay_ms(uint32_t ms);

#endif /* SOURCES_SYSTICK_H_ */
