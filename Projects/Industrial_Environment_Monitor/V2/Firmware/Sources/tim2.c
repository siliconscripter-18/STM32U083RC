/******************************************************************************
 * @file    tim2.c
 * @brief   TIM2 PWM driver for buzzer control.
 *
 *          Configures TIM2 CH1 on PA15 for buzzer tone generation and
 *          provides occupancy warning and thermal alarm functions.
 *
 * @author  Balaji M
 * @target  STM32U083RC
 ******************************************************************************/

#include "stm32u083xx.h"
#include "tim2.h"
#include "SysTick.h"


/******************************************************************************
 * @brief Initializes TIM2 CH1 for PWM buzzer output.
 ******************************************************************************/
void TIM2_PWM_Init(void)
{
    /* Enable GPIOA clock */
    RCC->IOPENR |= RCC_IOPENR_GPIOAEN;

    /* Enable TIM2 clock */
    RCC->APBENR1 |= RCC_APBENR1_TIM2EN;

    /* Configure PA15 as alternate function */
    GPIOA->MODER &= ~GPIO_MODER_MODE15_Msk;
    GPIOA->MODER |= GPIO_MODER_MODE15_1;

    /* PA15 = AF1 = TIM2_CH1 */
    GPIOA->AFR[1] &= ~GPIO_AFRH_AFSEL15_Msk;
    GPIOA->AFR[1] |= GPIO_AFRH_AFSEL15_0;

    /* Set initial PWM frequency configuration */
    TIM2->PSC = 3U;
    TIM2->ARR = 999U;

    /* Configure CH1 for PWM mode 1 */
    TIM2->CCMR1 &= ~TIM_CCMR1_OC1M_Msk;
    TIM2->CCMR1 |= TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2;

    /* Enable output compare preload */
    TIM2->CCMR1 |= TIM_CCMR1_OC1PE;

    /* Enable TIM2 CH1 output */
    TIM2->CCER &= ~TIM_CCER_CC1E_Msk;
    TIM2->CCER |= TIM_CCER_CC1E;

    /* Start with 0% duty */
    TIM2->CCR1 = 0U;

    /* Start timer */
    TIM2->CR1 |= TIM_CR1_CEN;
}


/******************************************************************************
 * @brief Sets the buzzer output frequency.
 *
 * @param frequency Desired buzzer frequency in Hz.
 ******************************************************************************/
void Buzzer_SetTone(uint16_t frequency)
{
    uint16_t ARR;
    uint16_t CCR1;

    ARR = (1000000U / frequency) - 1U;
    CCR1 = (ARR + 1U) / 2U;

    TIM2->ARR = ARR;
    TIM2->CCR1 = CCR1;

    /* Apply updated timer values */
    TIM2->EGR |= TIM_EGR_UG;

    /* Enable buzzer output */
    TIM2->CCER |= TIM_CCER_CC1E;
}


/******************************************************************************
 * @brief Stops the buzzer output.
 ******************************************************************************/
void Buzzer_Stop(void)
{
    TIM2->CCER &= ~TIM_CCER_CC1E_Msk;
}


/******************************************************************************
 * @brief Generates a periodic occupancy warning beep.
 *
 *        Produces a 1 kHz beep for 150 ms every 3 seconds.
 *        The function is non-blocking and should be called continuously.
 ******************************************************************************/
void Buzzer_OccupancyWarning(void)
{
    static uint32_t last_beep_tick = 0;
    static uint32_t start_tick = 0;
    static uint8_t active = 0;

    /* Start a new beep every 3 seconds */
    if (!active && ((SysTick_ms - last_beep_tick) >= 3000U))
    {
        last_beep_tick = SysTick_ms;
        Buzzer_SetTone(1000U);
        start_tick = SysTick_ms;
        active = 1;
    }

    /* Stop current beep after 150 ms */
    if (active && ((SysTick_ms - start_tick) >= 150U))
    {
        Buzzer_Stop();
        active = 0;
    }
}


/******************************************************************************
 * @brief Generates the thermal alarm tone sequence.
 *
 *        Cycles through 1 kHz, OFF, 2 kHz, OFF, and 3 kHz tones.
 *        Each step lasts 250 ms.
 *
 *        The function is non-blocking and should be called continuously.
 ******************************************************************************/
void Buzzer_ThermalAlarm(void)
{
    static uint32_t last_tick = 0;
    static uint8_t step = 0;

    /* Change alarm step every 250 ms */
    if ((SysTick_ms - last_tick) < 250U)
    {
        return;
    }

    last_tick = SysTick_ms;

    if (step == 0U)
    {
        Buzzer_SetTone(1000U);
    }
    else if (step == 1U)
    {
        Buzzer_Stop();
    }
    else if (step == 2U)
    {
        Buzzer_SetTone(2000U);
    }
    else if (step == 3U)
    {
        Buzzer_Stop();
    }
    else if (step == 4U)
    {
        Buzzer_SetTone(3000U);
    }
    else
    {
        Buzzer_Stop();
    }

    step++;

    if (step > 5U)
    {
        step = 0U;
    }
}
