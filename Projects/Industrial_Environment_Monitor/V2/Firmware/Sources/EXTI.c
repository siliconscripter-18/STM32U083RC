/******************************************************************************
 * @file    EXTI.c
 * @brief   External interrupt driver for IR occupancy sensors.
 *
 *          Configures PA0 and PA1 as falling-edge EXTI inputs for entry
 *          and exit detection. Software debounce is applied using SysTick.
 *
 *          PA0 -> S1 -> Entry
 *          PA1 -> S2 -> Exit
 *
 * @author  Balaji M
 * @target  STM32U083RC
 ******************************************************************************/

#include "stm32u083xx.h"
#include "EXTI.h"
#include "SysTick.h"

#define DEBOUNCE_MS    300U

volatile uint8_t S1_Flag = 0U;
volatile uint8_t S2_Flag = 0U;

/******************************************************************************
 * @brief Initializes EXTI0 and EXTI1 for falling-edge interrupts.
 *
 *        GPIOA is selected as the EXTI source for both sensor inputs.
 ******************************************************************************/
void EXTI_Init(void)
{
    /* Select GPIOA as the EXTI source */
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
 *        Pending interrupt flags are cleared and software debounce is
 *        applied using the SysTick millisecond counter.
 ******************************************************************************/
void EXTI0_1_IRQHandler(void)
{
    static uint32_t last_s1_tick = 0U;
    static uint32_t last_s2_tick = 0U;
    uint32_t now = SysTick_ms;

    /* S1 -> PA0 -> Entry */
    if (EXTI->FPR1 & EXTI_FPR1_FPIF0)
    {
        /* Clear pending interrupt flag */
        EXTI->FPR1 = EXTI_FPR1_FPIF0;

        /* Apply software debounce */
        if ((now - last_s1_tick) > DEBOUNCE_MS)
        {
            S1_Flag = 1U;
            last_s1_tick = now;
        }
    }

    /* S2 -> PA1 -> Exit */
    if (EXTI->FPR1 & EXTI_FPR1_FPIF1)
    {
        /* Clear pending interrupt flag */
        EXTI->FPR1 = EXTI_FPR1_FPIF1;

        /* Apply software debounce */
        if ((now - last_s2_tick) > DEBOUNCE_MS)
        {
            S2_Flag = 1U;
            last_s2_tick = now;
        }
    }
}
