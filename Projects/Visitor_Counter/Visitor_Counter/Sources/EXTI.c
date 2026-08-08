/******************************************************************************
 * @file    EXTI.c
 * @brief   External interrupt driver for IR sensor inputs.
 *
 * @author  Balaji M
 * @target  STM32U083RC
 ******************************************************************************/

#include <stdint.h>
#include "stm32u083xx.h"
#include "EXTI.h"
#include "SysTick.h"

#define DEBOUNCE_MS    150U

volatile uint8_t InFlag  = 0;
volatile uint8_t OutFlag = 0;

/******************************************************************************
 * @brief Initializes EXTI0 and EXTI1 for falling-edge interrupts.
 ******************************************************************************/
void EXTI_Init(void)
{
    /* Select GPIOA as the EXTI source for EXTI0 and EXTI1 */
    EXTI->EXTICR[0] &= ~EXTI_EXTICR1_EXTI0_Msk;
    EXTI->EXTICR[0] &= ~EXTI_EXTICR1_EXTI1_Msk;

    /* Enable falling-edge trigger for EXTI0 and EXTI1 */
    EXTI->FTSR1 |= EXTI_FTSR1_TR0 | EXTI_FTSR1_TR1;

    /* Unmask EXTI0 and EXTI1 interrupt lines */
    EXTI->IMR1 |= EXTI_IMR1_IM0 | EXTI_IMR1_IM1;

    /* Enable EXTI0_1 interrupt in the NVIC */
    NVIC_EnableIRQ(EXTI0_1_IRQn);
}

/******************************************************************************
 * @brief Handles EXTI0 and EXTI1 interrupts.
 *
 * @note Software debouncing is performed using the SysTick millisecond counter.
 ******************************************************************************/
void EXTI0_1_IRQHandler(void)
{
    static uint32_t last_in_tick  = 0;
    static uint32_t last_out_tick = 0;

    uint32_t now = SysTick_ms;

    /* Handle EXTI0 interrupt (Entry sensor) */
    if (EXTI->FPR1 & EXTI_FPR1_FPIF0)
    {
        /* Clear the pending interrupt flag */
        EXTI->FPR1 = EXTI_FPR1_FPIF0;

        /* Apply software debounce */
        if ((now - last_in_tick) > DEBOUNCE_MS)
        {
            InFlag = 1;
            last_in_tick = now;
        }
    }

    /* Handle EXTI1 interrupt (Exit sensor) */
    if (EXTI->FPR1 & EXTI_FPR1_FPIF1)
    {
        /* Clear the pending interrupt flag */
        EXTI->FPR1 = EXTI_FPR1_FPIF1;

        /* Apply software debounce */
        if ((now - last_out_tick) > DEBOUNCE_MS)
        {
            OutFlag = 1;
            last_out_tick = now;
        }
    }
}
