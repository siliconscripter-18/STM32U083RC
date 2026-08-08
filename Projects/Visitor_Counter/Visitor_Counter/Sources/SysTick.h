/******************************************************************************
 * @file    SysTick.h
 * @brief   SysTick driver interface.
 *
 * @author  Balaji M
 * @target  STM32U083RC
 ******************************************************************************/

#ifndef SYSTICK_H
#define SYSTICK_H

#include <stdint.h>

/* Millisecond system tick counter */
extern volatile uint32_t SysTick_ms;

/* Initializes the SysTick timer */
void SysTick_Init(void);

/* SysTick interrupt service routine */
void SysTick_Handler(void);

/* Generates a blocking delay */
void Delay_ms(uint32_t ms);

#endif /* SYSTICK_H */
