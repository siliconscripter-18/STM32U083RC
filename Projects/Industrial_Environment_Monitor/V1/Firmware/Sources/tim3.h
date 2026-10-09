/******************************************************************************
 * @file    tim3.h
 * @brief   TIM3 PWM driver interface for fan speed control.
 *
 *          Provides functions for initializing TIM3 PWM output and setting
 *          the fan PWM duty cycle.
 *
 * @author  Balaji M
 * @target  STM32U083RC
 ******************************************************************************/

#ifndef SOURCES_TIM3_H_
#define SOURCES_TIM3_H_

#include <stdint.h>


/******************************************************************************
 * @brief Initializes TIM3 CH1 for PWM fan control.
 ******************************************************************************/
void TIM3_PWM_Init(void);


/******************************************************************************
 * @brief Sets the fan PWM duty cycle.
 *
 * @param duty PWM compare value.
 ******************************************************************************/
void TIM3_PWM_SetDuty(uint16_t duty);

#endif /* SOURCES_TIM3_H_ */
