/******************************************************************************
 * @file    tim1.h
 * @brief   TIM1 timer driver interface.
 *
 *          Provides TIM1 initialization and blocking delay functions for
 *          microsecond and millisecond timing.
 *
 * @author  Balaji M
 * @target  STM32U083RC
 ******************************************************************************/

#ifndef SOURCES_TIM1_H_
#define SOURCES_TIM1_H_

#include <stdint.h>


/******************************************************************************
 * @brief Initializes TIM1 for 1 us timer resolution.
 ******************************************************************************/
void TIM1_Init(void);


/******************************************************************************
 * @brief Provides a blocking delay in microseconds.
 *
 * @param us Delay duration in microseconds.
 ******************************************************************************/
void delay_us(uint16_t us);


/******************************************************************************
 * @brief Provides a blocking delay in milliseconds.
 *
 * @param ms Delay duration in milliseconds.
 ******************************************************************************/
void delay_ms(uint16_t ms);

#endif /* SOURCES_TIM1_H_ */
