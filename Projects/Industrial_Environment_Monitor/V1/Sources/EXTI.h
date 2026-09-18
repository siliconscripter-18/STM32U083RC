#ifndef EXTI_H
#define EXTI_H

#include <stdint.h>

/******************************************************************************
 * @file    EXTI.h
 * @brief   External interrupt driver interface for IR occupancy sensors.
 *
 *          Provides the interface for EXTI0 and EXTI1 used to detect
 *          entry and exit events.
 *
 *          PA0 -> S1 -> Entry
 *          PA1 -> S2 -> Exit
 *
 * @author  Balaji M
 * @target  STM32U083RC
 ******************************************************************************/

/* Entry and exit event flags */
extern volatile uint8_t S1_Flag;
extern volatile uint8_t S2_Flag;

/******************************************************************************
 * @brief Initializes EXTI0 and EXTI1 for IR sensor inputs.
 ******************************************************************************/
void EXTI_Init(void);

/******************************************************************************
 * @brief EXTI0 and EXTI1 interrupt service routine.
 ******************************************************************************/
void EXTI0_1_IRQHandler(void);

#endif /* EXTI_H */
