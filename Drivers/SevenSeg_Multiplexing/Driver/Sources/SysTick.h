
/******************************************************************************
 * @file    SysTick.h
 * @brief   Function declarations for the SysTick driver.
 *
 *          Provides the millisecond system tick counter and interfaces
 *          for initializing SysTick and generating millisecond delays.
 *
 * @author  Balaji M
 * @target  STM32U083RC
 ******************************************************************************/

#ifndef SYSTICK_H
#define SYSTICK_H

#include <stdint.h>


/* Global millisecond system tick counter, updated by the SysTick interrupt */
extern volatile uint32_t SysTick_ms;


/******************************************************************************
 * @brief Initializes SysTick to generate a 1 ms periodic interrupt.
 ******************************************************************************/
void SysTick_Init(void);

#endif /* SYSTICK_H */
