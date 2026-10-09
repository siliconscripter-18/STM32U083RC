/******************************************************************************
 * @file    tim3.c
 * @brief   TIM3 PWM driver for fan speed control.
 *
 *          Configures TIM3 CH1 on PB4 for PWM output and provides a function
 *          to set the fan PWM duty cycle.
 *
 * @author  Balaji M
 * @target  STM32U083RC
 ******************************************************************************/

#include "stm32u083xx.h"
#include "tim3.h"


/******************************************************************************
 * @brief Initializes TIM3 CH1 for PWM fan control.
 ******************************************************************************/
void TIM3_PWM_Init(void)
{
    /* Enable GPIOB clock */
    RCC->IOPENR |= RCC_IOPENR_GPIOBEN;

    /* Enable TIM3 peripheral clock */
    RCC->APBENR1 |= RCC_APBENR1_TIM3EN;

    /* Configure PB4 as alternate function */
    GPIOB->MODER &= ~GPIO_MODER_MODE4_Msk;
    GPIOB->MODER |= GPIO_MODER_MODE4_1;

    /* PB4 = AF2 = TIM3_CH1 */
    GPIOB->AFR[0] &= ~GPIO_AFRL_AFSEL4_Msk;
    GPIOB->AFR[0] |= GPIO_AFRL_AFSEL4_1;

    /* Configure PWM frequency */
    TIM3->PSC = 3U;
    TIM3->ARR = 999U;

    /* Configure CH1 for PWM mode 1 */
    TIM3->CCMR1 &= ~TIM_CCMR1_OC1M_Msk;
    TIM3->CCMR1 |= TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2;

    /* Enable TIM3 CH1 output */
    TIM3->CCER &= ~TIM_CCER_CC1E_Msk;
    TIM3->CCER |= TIM_CCER_CC1E;

    /* Start with 0% duty */
    TIM3->CCR1 = 0U;

    /* Enable timer counter */
    TIM3->CR1 |= TIM_CR1_CEN;
}


/******************************************************************************
 * @brief Sets the fan PWM duty cycle.
 *
 * @param duty PWM compare value.
 ******************************************************************************/
void TIM3_PWM_SetDuty(uint16_t duty)
{
    TIM3->CCR1 = duty;
}
