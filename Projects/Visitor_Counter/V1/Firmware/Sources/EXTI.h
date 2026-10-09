/******************************************************************************
 * @file    EXTI.h
 * @brief   External interrupt driver interface.
 *
 * @author  Balaji M
 * @target  STM32U083RC
 ******************************************************************************/

#ifndef EXTI_H
#define EXTI_H

#include <stdint.h>

/* Entry and exit event flags */
extern volatile uint8_t InFlag;
extern volatile uint8_t OutFlag;

/* Initializes EXTI0 and EXTI1 */
void EXTI_Init(void);

/* EXTI0 and EXTI1 interrupt service routine */
void EXTI0_1_IRQHandler(void);

#endif /* EXTI_H */
