/******************************************************************************
 * @file    EXTI.h
 * @brief   External interrupt driver interface.
 *
 * @author  Balaji M
 * @target  STM32U083RC
 ******************************************************************************/

#ifndef EXTI_H
#define EXTI_H

/* Initializes EXTI13 for PC13 falling-edge detection */
void EXTI_Init(void);

/* EXTI4_15 interrupt service routine */
void EXTI4_15_IRQHandler(void);

#endif /* EXTI_H */