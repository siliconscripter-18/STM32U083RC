/******************************************************************************
 * @file    tim2.h
 * @brief   TIM2 PWM and buzzer control interface.
 *
 *          Provides functions for initializing TIM2 PWM output, generating
 *          buzzer tones, and controlling occupancy and thermal alarm patterns.
 *
 * @author  Balaji M
 * @target  STM32U083RC
 ******************************************************************************/

#ifndef SOURCES_TIM2_H_
#define SOURCES_TIM2_H_

#include <stdint.h>


/******************************************************************************
 * @brief Initializes TIM2 CH1 for PWM buzzer output.
 ******************************************************************************/
void TIM2_PWM_Init(void);


/******************************************************************************
 * @brief Sets the buzzer output frequency.
 *
 * @param frequency Desired buzzer frequency in Hz.
 ******************************************************************************/
void Buzzer_SetTone(uint16_t frequency);


/******************************************************************************
 * @brief Stops the buzzer output.
 ******************************************************************************/
void Buzzer_Stop(void);


/******************************************************************************
 * @brief Generates the periodic occupancy warning beep.
 ******************************************************************************/
void Buzzer_OccupancyWarning(void);


/******************************************************************************
 * @brief Generates the thermal alarm tone sequence.
 ******************************************************************************/
void Buzzer_ThermalAlarm(void);

#endif /* SOURCES_TIM2_H_ */
