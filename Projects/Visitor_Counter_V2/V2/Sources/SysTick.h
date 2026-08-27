/******************************************************************************
 * @file    SysTick.h
 * @brief   SysTick driver interface for the Visitor Counter application.
 *
 *          Provides the millisecond system tick counter and timing functions
 *          used for application delays, sensor debounce, FSM timeouts, and
 *          warning durations.
 *
 * @author  Balaji M
 * @target  STM32U083RC
 ******************************************************************************/

#ifndef SYSTICK_H
#define SYSTICK_H

#include <stdint.h>


/******************************************************************************
 * @brief Millisecond system tick counter.
 *
 *        Incremented by the SysTick interrupt handler once every millisecond.
 ******************************************************************************/
extern volatile uint32_t SysTick_ms;


/******************************************************************************
 * @brief Initializes the SysTick timer for a 1 ms system tick.
 ******************************************************************************/
void SysTick_Init(void);


/******************************************************************************
 * @brief SysTick interrupt service routine.
 ******************************************************************************/
void SysTick_Handler(void);


/******************************************************************************
 * @brief Generates a blocking delay.
 *
 * @param ms Delay duration in milliseconds.
 ******************************************************************************/
void Delay_ms(uint32_t ms);


#endif /* SYSTICK_H */
