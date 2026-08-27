/******************************************************************************
 * @file    EXTI.c
 * @brief   External interrupt driver for the four IR sensor inputs.
 *
 *          Configures falling-edge EXTI interrupts for the entry and exit
 *          sensors and provides independent debounce and re-arm handling for
 *          each sensor.
 *
 *          Sensor mapping:
 *          PA0 -> S1 (Entry)
 *          PA1 -> S2 (Entry)
 *          PA6 -> S3 (Exit)
 *          PA7 -> S4 (Exit)
 *
 * @author  Balaji M
 * @target  STM32U083RC
 ******************************************************************************/

#include <stdint.h>

#include "stm32u083xx.h"
#include "EXTI.h"
#include "SysTick.h"

#define DEBOUNCE_MS    500U


/* Sensor event flags consumed by the FSM */
volatile uint8_t S1_Flag = 0;
volatile uint8_t S2_Flag = 0;
volatile uint8_t S3_Flag = 0;
volatile uint8_t S4_Flag = 0;


/*
 * Sensor re-arm status.
 *
 * A sensor is accepted once and then locked until the beam becomes clear.
 * This prevents multiple interrupts while the same person remains in the
 * sensor path.
 */
static volatile uint8_t S1_Armed = 1;
static volatile uint8_t S2_Armed = 1;
static volatile uint8_t S3_Armed = 1;
static volatile uint8_t S4_Armed = 1;


/******************************************************************************
 * @brief Initializes EXTI interrupts for all four IR sensors.
 *
 *          All sensors use falling-edge detection. PA0 and PA1 are handled
 *          by EXTI0_1_IRQn, while PA6 and PA7 share EXTI4_15_IRQn.
 ******************************************************************************/
void EXTI_Init(void)
{
    /* Select GPIOA as the EXTI source for all four sensor inputs */

    /* EXTI0 -> PA0 (S1) */
    EXTI->EXTICR[0] &= ~EXTI_EXTICR1_EXTI0_Msk;

    /* EXTI1 -> PA1 (S2) */
    EXTI->EXTICR[0] &= ~EXTI_EXTICR1_EXTI1_Msk;

    /* EXTI6 -> PA6 (S3) */
    EXTI->EXTICR[1] &= ~EXTI_EXTICR2_EXTI6_Msk;

    /* EXTI7 -> PA7 (S4) */
    EXTI->EXTICR[1] &= ~EXTI_EXTICR2_EXTI7_Msk;


    /* Trigger interrupts on falling edges */
    EXTI->FTSR1 |= EXTI_FTSR1_TR0 |
                   EXTI_FTSR1_TR1 |
                   EXTI_FTSR1_TR6 |
                   EXTI_FTSR1_TR7;


    /* Unmask the four EXTI lines */
    EXTI->IMR1 |= EXTI_IMR1_IM0 |
                  EXTI_IMR1_IM1 |
                  EXTI_IMR1_IM6 |
                  EXTI_IMR1_IM7;


    /* Enable the corresponding NVIC interrupt groups */
    NVIC_EnableIRQ(EXTI0_1_IRQn);
    NVIC_EnableIRQ(EXTI4_15_IRQn);
}


/******************************************************************************
 * @brief Re-arms sensors after their beams become clear.
 *
 *          Sensors use pull-up inputs, therefore a HIGH input indicates that
 *          the beam is clear. The function is called continuously from the
 *          main loop so that a sensor can accept the next beam interruption.
 ******************************************************************************/
void EXTI_Process(void)
{
    /* Re-arm S1 when its beam becomes clear */
    if (GPIOA->IDR & GPIO_IDR_ID0)
    {
        S1_Armed = 1;
    }

    /* Re-arm S2 when its beam becomes clear */
    if (GPIOA->IDR & GPIO_IDR_ID1)
    {
        S2_Armed = 1;
    }

    /* Re-arm S3 when its beam becomes clear */
    if (GPIOA->IDR & GPIO_IDR_ID6)
    {
        S3_Armed = 1;
    }

    /* Re-arm S4 when its beam becomes clear */
    if (GPIOA->IDR & GPIO_IDR_ID7)
    {
        S4_Armed = 1;
    }
}


/******************************************************************************
 * @brief Handles EXTI0 and EXTI1 interrupts.
 *
 *          Generates one event per valid sensor activation while applying
 *          independent debounce timing to S1 and S2.
 ******************************************************************************/
void EXTI0_1_IRQHandler(void)
{
    static uint32_t last_s1_tick = 0;
    static uint32_t last_s2_tick = 0;

    uint32_t now = SysTick_ms;


    /* S1 -> PA0 */
    if (EXTI->FPR1 & EXTI_FPR1_FPIF0)
    {
        EXTI->FPR1 = EXTI_FPR1_FPIF0;

        if (S1_Armed &&
            ((now - last_s1_tick) >= DEBOUNCE_MS))
        {
            S1_Flag = 1;
            S1_Armed = 0;
            last_s1_tick = now;
        }
    }


    /* S2 -> PA1 */
    if (EXTI->FPR1 & EXTI_FPR1_FPIF1)
    {
        EXTI->FPR1 = EXTI_FPR1_FPIF1;

        if (S2_Armed &&
            ((now - last_s2_tick) >= DEBOUNCE_MS))
        {
            S2_Flag = 1;
            S2_Armed = 0;
            last_s2_tick = now;
        }
    }
}


/******************************************************************************
 * @brief Handles EXTI6 and EXTI7 interrupts.
 *
 *          Generates one event per valid sensor activation while applying
 *          independent debounce timing to S3 and S4.
 ******************************************************************************/
void EXTI4_15_IRQHandler(void)
{
    static uint32_t last_s3_tick = 0;
    static uint32_t last_s4_tick = 0;

    uint32_t now = SysTick_ms;


    /* S3 -> PA6 */
    if (EXTI->FPR1 & EXTI_FPR1_FPIF6)
    {
        EXTI->FPR1 = EXTI_FPR1_FPIF6;

        if (S3_Armed &&
            ((now - last_s3_tick) >= DEBOUNCE_MS))
        {
            S3_Flag = 1;
            S3_Armed = 0;
            last_s3_tick = now;
        }
    }


    /* S4 -> PA7 */
    if (EXTI->FPR1 & EXTI_FPR1_FPIF7)
    {
        EXTI->FPR1 = EXTI_FPR1_FPIF7;

        if (S4_Armed &&
            ((now - last_s4_tick) >= DEBOUNCE_MS))
        {
            S4_Flag = 1;
            S4_Armed = 0;
            last_s4_tick = now;
        }
    }
}
