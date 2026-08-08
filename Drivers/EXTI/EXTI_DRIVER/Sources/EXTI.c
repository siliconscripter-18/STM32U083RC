/******************************************************************************
 * @file    EXTI.c
 * @brief   External interrupt driver for EXTI13 on PC13.
 *
 * @author  Balaji M
 * @target  STM32U083RC
 ******************************************************************************/

#include "stm32u083xx.h"
#include "EXTI.h"

/******************************************************************************
 * @brief Initializes EXTI13 for a falling-edge interrupt from PC13.
 ******************************************************************************/
void EXTI_Init(void)
{
    /* Select GPIOC as the EXTI source for EXTI13 */
    EXTI->EXTICR[3] &= ~EXTI_EXTICR4_EXTI13_Msk;
    EXTI->EXTICR[3] |=  EXTI_EXTICR4_EXTI13_1;

    /* Enable falling-edge trigger for EXTI13 */
    EXTI->FTSR1 |= EXTI_FTSR1_TR13;

    /* Unmask EXTI13 interrupt */
    EXTI->IMR1 |= EXTI_IMR1_IM13;

    /* Enable EXTI4_15 interrupt in the NVIC */
    NVIC_EnableIRQ(EXTI4_15_IRQn);
}

/******************************************************************************
 * @brief Handles EXTI13 interrupt.
 *
 * The interrupt is generated when PC13 detects a falling edge.
 * PA5 is toggled as an indication that the interrupt was received.
 ******************************************************************************/
void EXTI4_15_IRQHandler(void)
{
    /* Check whether EXTI13 generated the interrupt */
    if (EXTI->FPR1 & EXTI_FPR1_FPIF13)
    {
        /* Clear the EXTI13 pending flag */
        EXTI->FPR1 = EXTI_FPR1_FPIF13;

        /* Toggle the LED connected to PA5 */
        GPIOA->ODR ^= GPIO_ODR_OD5;
    }
}
